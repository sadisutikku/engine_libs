//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "input/shared/keyboard/keyboard_base.h"

#include "engine/shared/events/events_manager.h"
#include "input/shared/input_events.h"
#include "input/shared/keyboard/key_codes.h"

namespace input
{
	void KEYBOARD_BASE::on_key_down( const KEY in_key )
	{
		engine::EVENTS_MANAGER::get_singleton()->add_event( std::make_unique<KEY_PRESSED_EVENT>( in_key, true ) );
	}

	void KEYBOARD_BASE::on_key_up( const KEY in_key )
	{
		engine::EVENTS_MANAGER::get_singleton()->add_event( std::make_unique<KEY_PRESSED_EVENT>( in_key, false ) );
	}
}
