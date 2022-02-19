
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_swapchain _base.h"

#include "core/platforms/include_wrappers/windows/windows.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/quad_d3d11.h"
#include "gui/shared/graphics/RHI/RHI_types.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace wrl = Microsoft::WRL;

//////////////////////////////////////////////////////////////////////
//! Forward Decelerations
//////////////////////////////////////////////////////////////////////

namespace core
{
	template<typename TYPE>
	struct VECTOR3D;
}

namespace gui
{
	class RENDERING_DEVICE_D3D11;

	class SWAPCHAIN_D3D11 : public SWAPCHAIN_BASE
	{
	public:
		SWAPCHAIN_D3D11(
			HWND window_handle,
			RENDERING_DEVICE_D3D11& rhi_device,
			float width,
			float height,
			RHI_Format format = RHI_Format_R8G8B8A8_Unorm,
			uint32_t flags = RHI_Present_Immediate,
			const char* name = nullptr
		);

		SWAPCHAIN_D3D11& operator = ( const SWAPCHAIN_D3D11& other ) = default;

		ID3D11RenderTargetView* Get_Resource_View_RenderTarget()  const { return m_render_target_view.Get(); }

		const QUAD_D3D11& get_viewport_quad() const { return m_viewport_quad; }

		bool resize( float width, float height, const bool force = false ) override;

		void clear_backbuffer( const core::VECTOR3D<float>& rgb );

		void present() override;

	//private:
		RENDERING_DEVICE_D3D11& m_device;
		wrl::ComPtr<IDXGISwapChain> m_swapchain;
		wrl::ComPtr<ID3D11RenderTargetView> m_render_target_view;
		QUAD_D3D11 m_viewport_quad;
	};
}
