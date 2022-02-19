//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "input/platforms/keyboard/windows/keyboard_windows.h"

#include "input/shared/keyboard/key_codes.h"

namespace input
{
	bool KEYBOARD_WINDOWS::handle_message(HWND in_window, uint32_t in_message, WPARAM in_wparam, LPARAM in_lparam)
	{
		const KEY key = KEY(in_wparam);

		switch (in_message)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			on_key_down(key);
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			on_key_up(key);
		}
		default:
		{
			return false;
		}
		}

		return true;
	}
}
