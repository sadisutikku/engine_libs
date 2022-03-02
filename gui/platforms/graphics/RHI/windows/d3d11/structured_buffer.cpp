//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/structured_buffer.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_context_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"

namespace gui
{
    void STRUCTURED_BUFFER_UAV_D3D11::create( RENDERING_DEVICE_D3D11& device, const uint32_t stride, const uint32_t element_count,
        const void* data, const bool make_output_accessible )
    {
        // Buffer that will hold the data
        D3D11_BUFFER_DESC desc = {};
        {
            desc.Usage = D3D11_USAGE_DEFAULT; // D3D11_USAGE_IMMUTABLE
            desc.ByteWidth = stride * element_count;
            desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
            desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            desc.StructureByteStride = stride;

            // Store initial data in buffer
            D3D11_SUBRESOURCE_DATA subresource_data = {};
            subresource_data.pSysMem = data;

            if ( FAILED( device.get_device()->CreateBuffer( &desc, data ? &subresource_data : nullptr, m_resource.GetAddressOf() ) ) )
            {
                ASSERT_FAILED( "CreateBuffer failed for GPU read/write buffer" );
                return;
            }
        }

        // UAV to access the data within the buffer created above
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
            desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.Buffer.FirstElement = 0;
            desc.Buffer.NumElements = element_count;

            if ( FAILED( device.get_device()->CreateUnorderedAccessView( m_resource.Get(), &desc, m_resource_uav.GetAddressOf() ) ) )
            {
                ASSERT_FAILED( "CreateUnorderedAccessView failed" );
                return;
            }
        }

        if ( make_output_accessible )
        {
            desc.Usage = D3D11_USAGE_STAGING;
            desc.BindFlags = 0u;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

            if ( FAILED( device.get_device()->CreateBuffer( &desc, nullptr, m_accessible_resource.GetAddressOf() ) ) )
            {
                ASSERT_FAILED( "CreateBuffer failed for cpu-accessible buffer" );
                return;
            }
        }
    }

    void* STRUCTURED_BUFFER_UAV_D3D11::map( RENDERING_DEVICE_D3D11& device, const MAPPING_TYPE mapping_type )
    {
        auto buffer = mapping_type == MAPPING_TYPE::GPU_WRITE_CPU_READ ? m_accessible_resource : m_resource;
        const auto map_type = mapping_type == MAPPING_TYPE::GPU_WRITE_CPU_READ ? D3D11_MAP_READ : D3D11_MAP_WRITE_DISCARD;

        ASSERT_TRUE( buffer.Get() );

        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        if ( FAILED( device.get_context()->m_device_context->Map( buffer.Get(), 0u, map_type, 0u, &mapped_resource ) ) )
        {
            ASSERT_FAILED( "Failed to map structured buffer." );
            return {};
        }

        return mapped_resource.pData;
    }

    void STRUCTURED_BUFFER_UAV_D3D11::unmap( RENDERING_DEVICE_D3D11& device )
    {
        ASSERT_TRUE( m_resource.Get() );

        device.get_context()->m_device_context->Unmap( m_resource.Get(), 0u );
    }

    void STRUCTURED_BUFFER_UAV_D3D11::copy_to_cpu_accessible_buffer( RENDERING_DEVICE_D3D11& device )
    {
        ASSERT_TRUE( m_resource.Get() );

        if ( m_accessible_resource )
        {
            device.get_context()->m_device_context->CopyResource( m_accessible_resource.Get(), m_resource.Get() );
        }
    }

    void STRUCTURED_BUFFER_SRV_D3D11::create( RENDERING_DEVICE_D3D11& device, const uint32_t stride, const uint32_t element_count,
        const void* data )
    {
        // Buffer that will hold the data
        D3D11_BUFFER_DESC desc = {};
        {
            desc.Usage = D3D11_USAGE_DEFAULT; // D3D11_USAGE_IMMUTABLE
            desc.ByteWidth = stride * element_count;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            desc.StructureByteStride = stride;

            // Store initial data in buffer
            D3D11_SUBRESOURCE_DATA subresource_data = {};
            subresource_data.pSysMem = data;

            if ( FAILED( device.get_device()->CreateBuffer( &desc, data ? &subresource_data : nullptr, m_resource.GetAddressOf() ) ) )
            {
                ASSERT_FAILED( "CreateBuffer failed for GPU read/write buffer" );
                return;
            }
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
        srv_desc.Format = DXGI_FORMAT_UNKNOWN;
        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        srv_desc.BufferEx.FirstElement = 0;
        srv_desc.BufferEx.Flags = 0;
        srv_desc.BufferEx.NumElements = element_count;

        if ( FAILED( device.get_device()->CreateShaderResourceView( m_resource.Get(), &srv_desc, m_resource_srv.GetAddressOf() ) ) )
        {
            ASSERT_FAILED( "CreateShaderResourceView failed" );
            return;
        }
    }
}
