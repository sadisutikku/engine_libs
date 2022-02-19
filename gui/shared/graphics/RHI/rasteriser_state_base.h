
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_types.h"

namespace gui
{
	class RASTERISER_STATE_BASE
	{
	public:
		RASTERISER_STATE_BASE( RHI_Cull_Mode cull_mode, RHI_Fill_Mode fill_mode, bool depth_clip_enabled, bool scissor_enabled, bool antialised_line_enabled, 
							   float depth_bias = 0.0f, float depth_bias_clamp = 0.0f, float depth_bias_slope_scaled = 0.0f, float line_width = 1.0f );

		RHI_Cull_Mode GetCullMode()     const { return m_cull_mode; }
		RHI_Fill_Mode GetFillMode()     const { return m_fill_mode; }
		bool GetDepthClipEnabled()      const { return m_depth_clip_enabled; }
		bool GetScissorEnabled()        const { return m_scissor_enabled; }
		bool GetAntialisedLineEnabled() const { return m_antialised_line_enabled; }
		float GetLineWidth()            const { return m_line_width; }
		float GetDepthBias()            const { return m_depth_bias; }
		float GetDepthBiasClamp()       const { return m_depth_bias_clamp; }
		float GetDepthBiasSlopeScaled() const { return m_depth_bias_slope_scaled; }

	protected:
		RHI_Cull_Mode m_cull_mode = RHI_Cull_Undefined;
		RHI_Fill_Mode m_fill_mode = RHI_Fill_Undefined;
		bool m_depth_clip_enabled = false;
		bool m_scissor_enabled = false;
		bool m_antialised_line_enabled = false;
		float m_depth_bias = 0.0f;
		float m_depth_bias_clamp = 0.0f;
		float m_depth_bias_slope_scaled = 0.0f;
		float m_line_width = 1.0f;
	};
}
