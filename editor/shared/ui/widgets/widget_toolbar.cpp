
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/ui/widgets/widget_toolbar.h"

#include "core/shared/math/rectangle.h"
#include "core/shared/patterns/scoped_value.h"
#include "editor/shared/events/editor_events.h"
#include "editor/shared/workspaces/editors/editor_viewport.h"
#include "engine/shared/application/application.h"
#include "engine/shared/game_framework/game_instance.h"
#include "engine/shared/game_framework/nodes/node_tree.h"
#include "gui/shared/events/render_events.h"
#include "gui/shared/imgui/imgui.h"
#include "input/shared/input_events.h"


namespace editor
{
	WIDGET_TOOLBAR::WIDGET_TOOLBAR()
	{
		m_title = "Toolbar";

		m_max_size = { 800.0f, 200.0f };
		m_bounds.set_dimensions( { 800.0f, 100.0f } );
	}

	bool WIDGET_TOOLBAR::receive_event( const engine::EVENT& in_event )
	{
		bool handled{ false };

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{
				handled = WIDGET::receive_event( in_event );
			}
			break;
		case gui::MARK_FOR_RENDER_EVENT_ID:
			{
				handled = WIDGET::receive_event( in_event );
			}
			break;
		case gui::RENDER_IMGUI_EVENT_ID:
			{
				if ( m_should_render )
				{
					handled = WIDGET::receive_event( in_event );
				}
			}
			break;
		}

		return handled;
	}

	void WIDGET_TOOLBAR::render()
	{
		if ( m_is_visible )
		{
			if ( ImGui::Button( "Create viewport editor" ) )
			{
				auto& application = engine::APPLICATION::get_singleton()->get_main_game_instance();
				application.add_node_to_branch( new EDITOR_VIEWPORT, engine::NODE_BRANCH_TYPE::EDITOR_ROOT );
			}
		}
	}
}
