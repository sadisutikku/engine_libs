
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/ui/widgets/widget_files.h"

#include "core/shared/math/rectangle.h"
#include "core/shared/patterns/scoped_value.h"
#include "editor/shared/events/editor_events.h"
#include "gui/shared/events/render_events.h"
#include "gui/shared/imgui/imgui.h"
#include "input/shared/input_events.h"

namespace editor
{
	WIDGET_FILE::WIDGET_FILE()
	{
		m_title = "Files";
	}

	bool WIDGET_FILE::receive_event( const engine::EVENT& in_event )
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

	void WIDGET_FILE::render()
	{
		if ( m_is_visible )
		{
			//core::SCOPED_VALUE scoped_value{ m_should_render, false };

			
		}
	}
}
