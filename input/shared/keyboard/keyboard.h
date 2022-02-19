//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/automatic_singleton.h"

#if PLATFORM_WINDOWS
#include "input/platforms/keyboard/windows/keyboard_windows.h"
using KEYBOARD_PLATFORM = input::KEYBOARD_WINDOWS;
#endif

namespace input
{
	class KEYBOARD : public KEYBOARD_PLATFORM, public core::AUTOMATIC_SINGLETON<KEYBOARD>
	{
	public:
		using KEYBOARD_PLATFORM::handle_message;

	//private:
		friend core::AUTOMATIC_SINGLETON<KEYBOARD>;
		KEYBOARD() = default;
		~KEYBOARD() override = default;
	};
}