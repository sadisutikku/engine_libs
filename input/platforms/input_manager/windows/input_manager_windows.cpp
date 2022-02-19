//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "input/platforms/input_manager/windows/input_manager_windows.h"

#include "engine/shared/events/events_manager.h"
#include "input/shared/keyboard/keyboard.h"
#include "input/shared/mouse/mouse.h"

namespace input
{
	INPUT_MANAGER_WINDOWS::INPUT_MANAGER_WINDOWS()
	{
		engine::EVENTS_MANAGER::s_event_processors.push_back( [this]( HWND in_window, uint32_t in_message, WPARAM in_wparam, LPARAM in_lparam ) -> bool
											  {
												  return handle_platform_message( in_window, in_message, in_wparam, in_lparam );
											  } );
	}

	bool INPUT_MANAGER_WINDOWS::handle_platform_message(HWND in_window, uint32_t in_message, WPARAM in_wparam, LPARAM in_lparam)
	{
		if (input::KEYBOARD::get_singleton().handle_message(in_window, in_message, in_wparam, in_lparam))
		{
			return true;
		}

		if (input::MOUSE::get_singleton().handle_message(in_window, in_message, in_wparam, in_lparam))
		{
			return true;
		}

		return false;
	}
}
