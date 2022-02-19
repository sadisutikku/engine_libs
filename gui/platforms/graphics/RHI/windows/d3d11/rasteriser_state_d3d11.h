
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/rasteriser_state_base.h"

#include "core/platforms/include_wrappers/windows/windows.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace gui
{
	class RENDERING_DEVICE_D3D11;

	class RASTERISER_STATE_D3D11 : public RASTERISER_STATE_BASE
	{
	public:
		RASTERISER_STATE_D3D11( RENDERING_DEVICE_D3D11& device, RHI_Cull_Mode cull_mode, RHI_Fill_Mode fill_mode, bool depth_clip_enabled, bool scissor_enabled, bool antialised_line_enabled,
								float depth_bias = 0.0f, float depth_bias_clamp = 0.0f, float depth_bias_slope_scaled = 0.0f, float line_width = 1.0f );
	
		ID3D11RasterizerState* get_rasterizer_state() const { return m_rasterizer_state.Get();  }

	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizer_state;
	};
}
