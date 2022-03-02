
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/platforms/include_wrappers/windows/windows.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/buffer_d3d11.h"

#include <d3d11.h>
#include <memory>
#include <wrl/client.h>

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace gui
{
    class RENDERING_DEVICE_D3D11;
}

namespace gui
{
    //////////////////////////////////////////////////////////////////////
    //! Definitions
    //////////////////////////////////////////////////////////////////////

    enum class MAPPING_TYPE : uint8_t
    {
        GPU_WRITE_CPU_READ,
        GPU_READ_WRITE
    };

    class STRUCTURED_BUFFER_UAV_D3D11
    {
    public:
        void create( RENDERING_DEVICE_D3D11& device, uint32_t stride, uint32_t element_count, const void* data = nullptr, bool make_output_accessible = false );

        void* map( RENDERING_DEVICE_D3D11& device, MAPPING_TYPE mapping_type = MAPPING_TYPE::GPU_READ_WRITE );
        void unmap( RENDERING_DEVICE_D3D11& device );

        void copy_to_cpu_accessible_buffer( RENDERING_DEVICE_D3D11& device );

        ID3D11Buffer* get_resource() const { return m_resource.Get(); }
        ID3D11UnorderedAccessView* get_resource_uav() const { return m_resource_uav.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_resource;
        Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_resource_uav;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_accessible_resource;
    };

    class STRUCTURED_BUFFER_SRV_D3D11
    {
    public:
        void create( RENDERING_DEVICE_D3D11& device, uint32_t stride, uint32_t element_count, const void* data = nullptr );

        ID3D11Buffer* get_resource() const { return m_resource.Get(); }
        ID3D11ShaderResourceView* get_resource_srv() const { return m_resource_srv.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_resource;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resource_srv;
    };
}