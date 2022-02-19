

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/rasteriser_state_d3d11.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/d3d11_types.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"

#include <cmath>

namespace gui
{
	RASTERISER_STATE_D3D11::RASTERISER_STATE_D3D11( RENDERING_DEVICE_D3D11& device, const RHI_Cull_Mode cull_mode, const RHI_Fill_Mode fill_mode, const bool depth_clip_enabled,
													const bool scissor_enabled, const bool antialised_line_enabled, const float depth_bias,
													const float depth_bias_clamp, const float depth_bias_slope_scaled, const float line_width )
		: RASTERISER_STATE_BASE( cull_mode, fill_mode, depth_clip_enabled, scissor_enabled, antialised_line_enabled, depth_bias, depth_bias_clamp,
								 depth_bias_slope_scaled, line_width )
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.CullMode = d3d11_cull_mode[m_cull_mode];
		desc.FillMode = d3d11_polygon_mode[m_fill_mode];
		desc.FrontCounterClockwise = false;
		desc.DepthBias = static_cast<UINT>(std::floor( m_depth_bias * (float)(1 << 24) ));
		desc.DepthBiasClamp = m_depth_bias_clamp;
		desc.SlopeScaledDepthBias = m_depth_bias_slope_scaled;
		desc.DepthClipEnable = m_depth_clip_enabled;
		desc.MultisampleEnable = false;
		desc.AntialiasedLineEnable = m_antialised_line_enabled;
		desc.ScissorEnable = m_scissor_enabled;

		device.get_device()->CreateRasterizerState( &desc, m_rasterizer_state.GetAddressOf() );
	}
}
