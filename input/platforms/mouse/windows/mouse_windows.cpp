//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "input/platforms/mouse/windows/mouse_windows.h"

//#include "core/shared/patterns/delegate.h"
#include "input/shared/mouse/mouse_codes.h"

#include <WindowsX.h>

#include <iostream>
#include <sstream>

namespace input
{
	struct MOUSE_WINDOWS::IMPL
	{
		HWND m_window;
		LPARAM m_lparam;
	};
}

namespace input
{
	MOUSE_WINDOWS::MOUSE_WINDOWS()
		: m_impl( std::make_unique<IMPL>() )
	{}

	MOUSE_WINDOWS::~MOUSE_WINDOWS() = default;

	bool MOUSE_WINDOWS::handle_message(HWND in_window, uint32_t in_message, WPARAM in_wparam, LPARAM in_lparam)
	{
		m_impl->m_window = in_window;
		m_impl->m_lparam = in_lparam;

		switch (in_message)
		{
		case WM_LBUTTONDOWN:
		{
			on_mouse_button_down(MOUSE_BUTTON::LEFT);
			return true;
		}
		case WM_LBUTTONUP:
		{
			on_mouse_button_up(MOUSE_BUTTON::LEFT);
			return true;
		}
		case WM_RBUTTONDOWN:
			{
				on_mouse_button_down( MOUSE_BUTTON::RIGHT );
				return true;
			}
		case WM_RBUTTONUP:
			{
				on_mouse_button_up( MOUSE_BUTTON::RIGHT );
				return true;
			}
		case WM_MOUSEMOVE:
		{
			handle_absolute_input( in_lparam );
			on_mouse_moved();
			return true;
		}
		case WM_MOUSEWHEEL:
			{
				on_mouse_wheel_scrolled_vertical( in_window, in_wparam, in_lparam );
				return true;
			}
		case WM_INPUT:
		{			
			if (handle_raw_input())
			{
				on_mouse_moved();
			}
			return true;
		}
		default:
		{
			return false;
		}
		}

		return true;
	}

	void MOUSE_WINDOWS::handle_absolute_input( LPARAM lparam )
	{
		m_previous_position = m_current_position;

		const MOUSE_POS_TYPE new_mouse_position( static_cast<MOUSE_AXIS_TYPE>(GET_X_LPARAM( lparam )),
												 static_cast<MOUSE_AXIS_TYPE>(GET_Y_LPARAM( lparam )) );

		m_current_position = new_mouse_position;

		m_pos_delta = m_current_position - m_previous_position;
	}

	bool MOUSE_WINDOWS::handle_raw_input()
	{
		UINT size;
		// get the size of the input data
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(m_impl->m_lparam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
		{
			return false;
		}

		std::vector<char> raw_buffer;
		raw_buffer.resize(size);

		// then read in the input data
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(m_impl->m_lparam), RID_INPUT, raw_buffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
		{
			return false;
		}

		// process the raw input data
		const RAWINPUT& raw_input = reinterpret_cast<const RAWINPUT&>(*raw_buffer.data());
		if (raw_input.header.dwType == RIM_TYPEMOUSE // only listen to mouse type messages
			&& (raw_input.data.mouse.lLastX != 0 || raw_input.data.mouse.lLastY != 0))
		{
			m_pos_delta = MOUSE_POS_TYPE(raw_input.data.mouse.lLastX, raw_input.data.mouse.lLastY);
			std::wostringstream os_;
			os_ << m_pos_delta.x << "\n";
			OutputDebugStringW(os_.str().c_str());
			//OutputDebugString("Stuff: " << m_pos_delta.X);
			return true;
		}

		return false;
	}

	void MOUSE_WINDOWS::on_mouse_button_down(const MOUSE_BUTTON in_pressed_button)
	{
		super::on_mouse_button_down(in_pressed_button);

		while (::ShowCursor(FALSE) >= 0);
	}

	void MOUSE_WINDOWS::on_mouse_button_up(const MOUSE_BUTTON in_released_button)
	{
		super::on_mouse_button_up(in_released_button);

		while (::ShowCursor(TRUE) < 0);
	}

	void MOUSE_WINDOWS::on_mouse_wheel_scrolled_vertical( HWND window, WPARAM wparam, LPARAM lparam )
	{
		const int16_t wheel_delta = GET_WHEEL_DELTA_WPARAM( wparam );

		// Convert the mouse position from absolute to relative to the window origin
		POINT point;
		point.x = GET_X_LPARAM( lparam );
		point.y = GET_Y_LPARAM( lparam );
		ScreenToClient( window, &point );

		on_mouse_wheel_scrolled( { static_cast<MOUSE_AXIS_TYPE>(point.x), static_cast<MOUSE_AXIS_TYPE>(point.y) }, {}, { 0, wheel_delta } );
	}
}
