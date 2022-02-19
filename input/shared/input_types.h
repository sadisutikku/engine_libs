//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Types
//////////////////////////////////////////////////////////////////////

namespace input
{
	enum class EINPUT_TYPE : uint8_t
	{
		INVALID,

		MOUSE_MOVEMENT_VERTICAL,
		MOUSE_MOVEMENT_HORIZONTAL,
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		KEY_PRESSED,
		KEY_RELEASED,
		TRIGGER_PULLED,
	};
}
