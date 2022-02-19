


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/buffer_d3d11.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_context_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"

#include <d3d11.h>

namespace gui
{
	void BUFFER_D3D11::_destroy()
	{
		if ( m_buffer )
		{
			m_buffer->Release();
		}
	}

	bool BUFFER_D3D11::_create( RENDERING_DEVICE_BASE& device, const BUFFER_TYPE buffer_type, const void* data )
	{
		RENDERING_DEVICE_D3D11& device_d3d11 = static_cast<RENDERING_DEVICE_D3D11&>(device);

		const bool is_dynamic = data == nullptr;

		// Destroy previous buffer
		_destroy();

		// fill in a buffer description.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.Usage = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
		buffer_desc.CPUAccessFlags = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		buffer_desc.BindFlags = static_cast<uint32_t>(buffer_type);
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		switch ( buffer_type )
		{
		case BUFFER_TYPE::CONSTANT:
			{
				buffer_desc.ByteWidth = m_stride;
			}
			break;
		case BUFFER_TYPE::VERTEX:
		case BUFFER_TYPE::INDEX:
			{
				buffer_desc.ByteWidth = m_total_size;
			}
			break;
		}

		// fill in the subresource data.
		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = data;
		init_data.SysMemPitch = 0;
		init_data.SysMemSlicePitch = 0;

		const auto result = device_d3d11.get_device()->CreateBuffer( &buffer_desc, is_dynamic ? nullptr : &init_data, m_buffer.GetAddressOf() );
		if ( FAILED( result ) )
		{
			ASSERT_FAILED( "Failed to create vertex buffer" );
			return false;
		}

		return true;
	}

	void* BUFFER_D3D11::Map( RENDERING_DEVICE_D3D11& device )
	{
		if ( !m_buffer )
		{
			ASSERT_SHOULDNT_BE_HERE();
			return nullptr;
		}

		// Disable GPU access to the vertex buffer data.
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		const auto result = device.get_context()->m_device_context->Map( m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource );
		if ( FAILED( result ) )
		{
			ASSERT_FAILED( "Failed to map vertex buffer" );
			return nullptr;
		}

		return mapped_resource.pData;
	}

	bool BUFFER_D3D11::Unmap( RENDERING_DEVICE_D3D11& device )
	{
		if ( !m_buffer )
		{
			ASSERT_SHOULDNT_BE_HERE();
			return false;
		}

		// Re-enable GPU access to the vertex buffer data.
		device.get_context()->m_device_context->Unmap( m_buffer.Get(), 0u );
		return true;
	}
}
