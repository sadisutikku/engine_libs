
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/ui/widgets/widget_assets.h"

#include "core/shared/math/rectangle.h"
#include "core/shared/patterns/scoped_value.h"
#include "editor/shared/events/editor_events.h"
#include "editor/shared/ui/widgets/widget_files.h"
#include "engine/shared/events/events_manager.h"
#include "gui/shared/events/render_events.h"
#include "gui/shared/imgui/imgui.h"
#include "input/shared/input_events.h"

namespace
{
	// Events that can be received from an EVENT_MANAGER
	const core::VECTOR<core::UNIQUE_ID> s_broadcastable_events
	{
		{ input::MOUSE_MOVE_EVENT_ID }
	};
}

namespace editor
{
	WIDGET_ASSETS::WIDGET_ASSETS()
	{
		m_title = "Assets";

		m_max_size = { 1000.0f, 800.0f };
		m_bounds.set_dimensions( { 700.0f, 100.0f } );

		m_children.emplace_back( new WIDGET_FILE );

		engine::EVENTS_MANAGER::get_singleton()->add_events_receiver( this, s_broadcastable_events );
	}

	WIDGET_ASSETS::~WIDGET_ASSETS()
	{
		engine::EVENTS_MANAGER::get_singleton()->remove_events_receiver( this );
	}

	bool WIDGET_ASSETS::receive_event( const engine::EVENT& in_event )
	{
		bool handled{ false };

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{
				handled = WIDGET::receive_event( in_event );
			}
			break;
		case input::MOUSE_MOVE_EVENT_ID:
			{
				if ( WIDGET::receive_event( in_event ) )
				{
					receive_event( { gui::MARK_FOR_RENDER_EVENT_ID } );
				}
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

	void WIDGET_ASSETS::render()
	{
		if ( m_is_visible )
		{
			//core::SCOPED_VALUE scoped_value{ m_should_render, false };


		}
	}
}
