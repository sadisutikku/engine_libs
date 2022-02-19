
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/pipeline_state_base.h"

#include "core/shared/type_wrappers/array.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/texture2d_d3d11.h"

#include <d3d11.h>

namespace gui
{
	class SHADER_D3D11;
	class SWAPCHAIN_D3D11;
	class RASTERISER_STATE_D3D11;

	struct PIPELINE_STATE_D3D11 : public PIPELINE_STATE_BASE
	{
		SHADER_D3D11* m_vertex_shader{};
		SHADER_D3D11* m_pixel_shader{};
		SHADER_D3D11* m_compute_shader{};
		RASTERISER_STATE_D3D11* m_rasteriser_state{};
		SWAPCHAIN_D3D11* m_swapchain{};
		core::ARRAY<ID3D11RenderTargetView*, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> m_render_targets{};
	};
}
