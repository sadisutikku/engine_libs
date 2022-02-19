//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/events/event_base.h"

#include "core/shared/math/vector2d.h"
#include "core/shared/patterns/unique_id.h"
#include "input/shared/input_forward_declares.h"
#include "input/shared/keyboard/key_codes.h"
#include "input/shared/mouse/mouse_codes.h"

namespace input
{
	inline constexpr core::UNIQUE_ID MOUSE_MOVE_EVENT_ID{ 'MoMo' };
	inline constexpr core::UNIQUE_ID MOUSE_SCROLL_EVENT_ID{ 'MoSc' };
	inline constexpr core::UNIQUE_ID MOUSE_BUTTON_PRESSED_EVENT_ID{ 'MoBP' };

	inline constexpr core::UNIQUE_ID KEY_PRESSED_EVENT_ID{ 'KePr' };

	struct MOUSE_MOVE_EVENT : public engine::EVENT
	{
		MOUSE_MOVE_EVENT( const MOUSE_POS_TYPE& in_position, const MOUSE_POS_TYPE& in_delta )
			: engine::EVENT{ MOUSE_MOVE_EVENT_ID }
			, current_position{ in_position }
			, delta{ in_delta }
		{}

		MOUSE_POS_TYPE current_position;
		MOUSE_POS_TYPE delta;
	};

	struct MOUSE_SCROLL_EVENT : public engine::EVENT
	{
		MOUSE_SCROLL_EVENT( const MOUSE_POS_TYPE& in_position, const core::VECTOR2D<int16_t>& in_scroll_delta )
			: engine::EVENT{ MOUSE_SCROLL_EVENT_ID }
			, current_position{ in_position }
			, scroll_delta{ in_scroll_delta }
		{}

		MOUSE_POS_TYPE current_position;
		core::VECTOR2D<int16_t> scroll_delta;
	};

	struct MOUSE_BUTTON_PRESSED_EVENT : public engine::EVENT
	{
		MOUSE_BUTTON_PRESSED_EVENT( const MOUSE_POS_TYPE& in_position, input::MOUSE_BUTTON in_mouse_button, bool in_pressed )
			: engine::EVENT{ MOUSE_BUTTON_PRESSED_EVENT_ID }
			, current_position{ in_position }
			, mouse_button{ in_mouse_button }
			, pressed{ in_pressed }
		{}

		MOUSE_POS_TYPE current_position;
		input::MOUSE_BUTTON mouse_button;
		bool pressed;
	};

	struct KEY_PRESSED_EVENT : public engine::EVENT
	{
		KEY_PRESSED_EVENT( input::KEY in_key, bool in_pressed )
			: engine::EVENT{ KEY_PRESSED_EVENT_ID }
			, key{ in_key }
			, pressed{ in_pressed }
		{}

		input::KEY key;
		bool pressed;
	};
}
