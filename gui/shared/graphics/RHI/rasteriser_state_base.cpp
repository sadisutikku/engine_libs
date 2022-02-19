

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/rasteriser_state_base.h"

#include "core/shared/debugging/asserts.h"

namespace gui
{
	RASTERISER_STATE_BASE::RASTERISER_STATE_BASE( const RHI_Cull_Mode cull_mode, const RHI_Fill_Mode fill_mode, const bool depth_clip_enabled,
												  const bool scissor_enabled, const bool antialised_line_enabled, const float depth_bias,
												  const float depth_bias_clamp, const float depth_bias_slope_scaled, const float line_width )
		: m_cull_mode{ cull_mode }
		, m_fill_mode{ fill_mode }
		, m_depth_clip_enabled{ depth_clip_enabled }
		, m_scissor_enabled{ scissor_enabled }
		, m_antialised_line_enabled{ antialised_line_enabled }
		, m_depth_bias{ depth_bias }
		, m_depth_bias_clamp{ depth_bias_clamp }
		, m_depth_bias_slope_scaled{ depth_bias_slope_scaled }
		, m_line_width{ line_width }
	{}
}
