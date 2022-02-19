//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "input/shared/keyboard/keyboard_base.h"

#include "core/platforms/include_wrappers/windows/windows.h"

#include <stdint.h>

namespace input
{
	class KEYBOARD_WINDOWS : public KEYBOARD_BASE
	{
	protected:
		KEYBOARD_WINDOWS() = default;
		~KEYBOARD_WINDOWS() override = default;

		bool handle_message(HWND in_window, uint32_t in_message, WPARAM in_wparam, LPARAM in_lparam);
	};
}
