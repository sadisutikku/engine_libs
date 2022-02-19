//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/ui/widgets/widget.h"

#include "core/shared/math/rectangle.h"
#include "engine/shared/events/event_base.h"
#include "gui/shared/events/render_events.h"
#include "gui/shared/imgui/imgui.h"
#include "input/shared/input_events.h"

#include <gui/thirdparty/imgui/imgui_internal.h>

namespace gui
{
	WIDGET::WIDGET()
		: m_max_size{ core::FVECTOR2D::MAX_VALUE }
	{
		m_should_render = m_is_visible = true; // temp
	}

	bool WIDGET::receive_event( const engine::EVENT& in_event )
	{
		auto handled = engine::NODE::receive_event( in_event );

		switch ( in_event.unique_id )
		{
		case input::MOUSE_MOVE_EVENT_ID:
			{
				auto scroll_event = static_cast<const input::MOUSE_MOVE_EVENT&>(in_event);

				if ( m_bounds.is_within( /* todo: make events window-specific or use screen-relative mouse position */ scroll_event.current_position ) )
				{
					return true; // should be handled by child class(es)
				}
			}
			break;
		case input::MOUSE_BUTTON_PRESSED_EVENT_ID:
			{
				auto scroll_event = static_cast<const input::MOUSE_BUTTON_PRESSED_EVENT&>(in_event);

				if ( m_bounds.is_within( /* todo: make events window-specific or use screen-relative mouse position */ scroll_event.current_position ) )
				{
					return true; // should be handled by child class(es)
				}
			}
			break;
		case input::MOUSE_SCROLL_EVENT_ID:
			{
				auto scroll_event = static_cast<const input::MOUSE_SCROLL_EVENT&>(in_event);

				if ( m_bounds.is_within( /* todo: make events window-specific or use screen-relative mouse position */ scroll_event.current_position ) )
				{
					return true; // should be handled by child class(es)
				}
			}
			break;
		case gui::MARK_FOR_RENDER_EVENT_ID:
			{
				m_should_render = true;
			}
			break;
		case gui::RENDER_IMGUI_EVENT_ID:
			{
				if ( m_is_visible )
				{
					if ( m_position_override != core::FVECTOR2D::ZERO_VALUE )
					{
						ImGui::SetNextWindowPos( { m_position_override.x, m_position_override.y } );
					}

					if ( m_padding != core::FVECTOR2D::ZERO_VALUE )
					{
						ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { m_padding.x, m_padding.y } );
						++m_var_pushes;
					}

					if ( m_alpha.has_value() )
					{
						ImGui::PushStyleVar( ImGuiStyleVar_Alpha, m_alpha.value() );
						++m_var_pushes;
					}
					
					if ( m_size_override != core::FVECTOR2D::ZERO_VALUE )
					{
						ImGui::SetNextWindowSize( { m_size_override.x, m_size_override.y }, ImGuiCond_FirstUseEver );
					}

					if ( m_min_size != core::FVECTOR2D::ZERO_VALUE || m_max_size != core::FVECTOR2D::MAX_VALUE )
					{
						ImGui::SetNextWindowSizeConstraints( { m_min_size.x, m_min_size.y }, { m_max_size.x, m_max_size.y } );
					}

					bool begun{ false };

					if ( ImGui::Begin( m_title.c_str(), &m_is_visible, m_flags ) )
					{
						begun = true;

						m_window = ImGui::GetCurrentWindow();

						m_bounds = { m_window->Pos.x, m_window->Pos.y, m_window->ContentSize.x, m_window->ContentSize.y };
					}
					else if ( m_window && m_window->Hidden )
					{
						// Enters here if the window is hidden as part of an unselected tab.
						// ImGui::Begin() makes the window but returns false, then ImGui still expects ImGui::End() to be called.
						// So we make sure that when Widget::End() is called, ImGui::End() get's called as well.
						// Note: ImGui's docking is in beta, so maybe it's at fault here ?
						begun = true;
					}

					if ( m_window && m_window->Appearing )
					{
						//OnShow();
					}
					else if ( !m_is_visible )
					{
						//OnHide();
					}

					if ( begun )
					{
						render();

						ImGui::End();

						ImGui::PopStyleVar( m_var_pushes );
						m_var_pushes = 0;
					}
				}
			}
			break;
		}

		return handled;
	}
}
