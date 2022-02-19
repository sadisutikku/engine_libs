

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/command_list_d3d11.h"

#include "gui/platforms/graphics/RHI/windows/d3d11/pipeline_state_d3d11.h"

namespace gui
{
	bool COMMAND_LIST_D3D11::begin_render_pass( PIPELINE_STATE_D3D11 pipeline_state )
	{


		return true;
	}

	bool COMMAND_LIST_D3D11::DrawIndexed( const uint32_t index_count, const uint32_t index_offset, const uint32_t vertex_offset )
	{
		/*m_rhi_device->GetContextRhi()->device_context->DrawIndexed
		(
			static_cast<UINT>(index_count),
			static_cast<UINT>(index_offset),
			static_cast<INT>(vertex_offset)
		);*/

		return true;
	}
}
