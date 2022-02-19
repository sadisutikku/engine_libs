
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_device_base.h"

#include "core/platforms/include_wrappers/windows/windows.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_swapchain_d3d11.h"
#include "gui/shared/graphics/window_resources/window_resources_base.h"

#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
#include <unordered_map>

namespace wrl = Microsoft::WRL;

namespace gui
{
	class RENDERING_CONTEXT_D3D11;

	struct WINDOW_RESOURCES_D3D11
	{
		WINDOW_RESOURCES_BASE base_resources;
		SWAPCHAIN_D3D11 swapchain;
	};

	class RENDERING_DEVICE_D3D11 : public RENDERING_DEVICE_BASE
	{
	public:
		RENDERING_DEVICE_D3D11();
		~RENDERING_DEVICE_D3D11();

		bool ValidateResolution( uint32_t width, uint32_t height ) const;

		ID3D11Device* get_device() const { return m_device.Get(); }

		RENDERING_CONTEXT_D3D11* get_context() const { return m_context.get(); }

	private:
		void find_adapter();

		void create_logical_device();

		wrl::ComPtr<ID3D11Device> m_device;
		std::unique_ptr<RENDERING_CONTEXT_D3D11> m_context; //! @todo may want to bring the d3d11 context into this class, leaving the rest
	};
}
