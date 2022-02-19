
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/nodes/gui_node.h"

#include "gui/shared/events/render_events.h"
#include "gui/shared/graphics/renderer/renderer.h"

namespace gui
{
	GUI_NODE::GUI_NODE()
	{
		m_node_type = engine::NODE_BRANCH_TYPE::GRAPHICS_ROOT;
	}

	GUI_NODE::~GUI_NODE() = default;

	bool GUI_NODE::receive_event( const engine::EVENT& in_event )
	{
		auto handled{ false };

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{
				handled = NODE::receive_event( in_event );

				receive_event( { RENDER_EVENT_ID } );
			}
			break;
		case RENDER_EVENT_ID:
			{
				handled = NODE::receive_event( in_event );

				RENDERER::get_singleton()->pass_main();
			}
			break;
		}

		return handled;
	}
}
