//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "input/shared/mouse/mouse_base.h"

#include "core/shared/math/rectangle.h"
#include "engine/shared/events/events_manager.h"
#include "input/shared/input_events.h"
#include "input/shared/input_types.h"
#include "input/shared/mouse/mouse_codes.h"

namespace input
{
	struct INPUT_CONTAINER
	{
		EINPUT_TYPE m_input_type{ EINPUT_TYPE::INVALID };
		MOUSE_BUTTON m_button{ MOUSE_BUTTON::INVALID };
		float m_input_value{};
	};

	void determine_input_types( const core::FVECTOR2D& in_position_delta, std::vector<INPUT_CONTAINER>& out_input_containers )
	{
		if ( in_position_delta.x != 0 )
		{
			out_input_containers.push_back( { EINPUT_TYPE::MOUSE_MOVEMENT_HORIZONTAL, MOUSE_BUTTON::INVALID, in_position_delta.x } );
		}

		if ( in_position_delta.y != 0 )
		{
			out_input_containers.push_back( { EINPUT_TYPE::MOUSE_MOVEMENT_VERTICAL, MOUSE_BUTTON::INVALID, in_position_delta.y } );
		}
	}
	void MOUSE_BASE::on_mouse_moved()
	{
		// todo get the window that currently has focus, and maybe cache it
		/*gui::WINDOW* active_window = application::ENGINE::get_engine_window();
		const math::RECTANGLE<int32_t>& window_bounds = active_window->get_bounds();

		const float pos_delta_norm_x = static_cast<float>( m_pos_delta.X ) / static_cast<float>( window_bounds.get_width() );
		const float pos_delta_norm_y = static_cast<float>( m_pos_delta.Y ) / static_cast<float>( window_bounds.get_height() );
		m_pos_delta_norm = { pos_delta_norm_x, pos_delta_norm_y };*/

		std::vector<INPUT_CONTAINER> input_containers;
		determine_input_types( m_pos_delta_norm, input_containers );

		engine::EVENTS_MANAGER::get_singleton()->add_event( std::make_unique<MOUSE_MOVE_EVENT>( m_current_position, m_pos_delta ) );
	}

	void MOUSE_BASE::on_mouse_button_down(MOUSE_BUTTON in_pressed_button)
	{
		switch (in_pressed_button)
		{
		case MOUSE_BUTTON::LEFT:
		{
			m_left_mouse_down = true;
			break;
		}
		case MOUSE_BUTTON::RIGHT:
		{
			m_right_mouse_down = true;
			break;
		}
		case MOUSE_BUTTON::MIDDLE:
		{
			m_middle_mouse_down = true;
			break;
		}
		default:
		{
			return;
		}
		}

		engine::EVENTS_MANAGER::get_singleton()->add_event( std::make_unique<MOUSE_BUTTON_PRESSED_EVENT>( m_current_position, in_pressed_button, true ) );

		m_button_up_down.m_inner.execute<void, MOUSE_BUTTON, bool>(std::move(in_pressed_button), true);
	}

	void MOUSE_BASE::on_mouse_button_up(MOUSE_BUTTON in_released_button)
	{
		switch (in_released_button)
		{
		case MOUSE_BUTTON::LEFT:
		{
			m_left_mouse_down = false;
			break;
		}
		case MOUSE_BUTTON::RIGHT:
		{
			m_right_mouse_down = false;
			break;
		}
		case MOUSE_BUTTON::MIDDLE:
		{
			m_middle_mouse_down = false;
			break;
		}
		default:
		{
			return;
		}
		}

		engine::EVENTS_MANAGER::get_singleton()->add_event( std::make_unique<MOUSE_BUTTON_PRESSED_EVENT>( m_current_position, in_released_button, false ) );

		m_button_up_down.m_inner.execute<void, MOUSE_BUTTON, bool>(std::move(in_released_button), false);
	}

	void MOUSE_BASE::on_mouse_wheel_scrolled( const MOUSE_POS_TYPE& mouse_position, const core::VECTOR2D<int8_t>& step_delta,
											  const core::VECTOR2D<int16_t>& raw_delta )
	{
		engine::EVENTS_MANAGER::get_singleton()->add_event( std::make_unique<MOUSE_SCROLL_EVENT>( mouse_position, raw_delta ) );
	}
}
