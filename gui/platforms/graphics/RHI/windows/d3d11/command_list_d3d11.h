
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/command_list_base.h"

namespace gui
{
	struct PIPELINE_STATE_D3D11;

	class COMMAND_LIST_D3D11 : public COMMAND_LIST_BASE
	{
	public:
		bool begin_render_pass( PIPELINE_STATE_D3D11 pipeline_state );

		bool DrawIndexed( uint32_t index_count, uint32_t index_offset, uint32_t vertex_offset );
	};
}
