//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace input
{
	enum class KEY : uint8_t;
	struct WINDOWS_KEYS_STATE;
}

namespace input
{
	class KEYBOARD_BASE
	{
	protected:
		KEYBOARD_BASE() = default;
		virtual ~KEYBOARD_BASE() = default;

		//virtual void on_key_down( KEY in_key, const WINDOWS_KEYS_STATE& in_windows_keys_state ) = 0;

		void on_key_down( KEY in_key );
		void on_key_up( KEY in_key );
	};
}