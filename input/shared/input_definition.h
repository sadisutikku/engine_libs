//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/debugging/asserts.h"
#include "input/shared/input_types.h"
#include "input/shared/keyboard/key_codes.h"
#include "input/shared/mouse/mouse_codes.h"

#include <functional>

//////////////////////////////////////////////////////////////////////
//! Types
//////////////////////////////////////////////////////////////////////

namespace input
{
	enum class INPUT_BINDING_TYPE : uint8_t
	{
		INVALID,

		AXIS,
		ACTION
	};

	struct INPUT_BINDING
	{
		INPUT_BINDING(EINPUT_TYPE in_input_type)
			: m_input_type(in_input_type)
		{}

		virtual INPUT_BINDING_TYPE get_binding_type() const = 0;

		bool operator==(const INPUT_BINDING& in_other) const
		{
			return m_input_type == in_other.m_input_type;
		}

		EINPUT_TYPE m_input_type{ EINPUT_TYPE::INVALID };
	};

	struct AXIS_INPUT_BINDING : public INPUT_BINDING
	{
		AXIS_INPUT_BINDING(EINPUT_TYPE in_input_type, MOUSE_BUTTON in_button = MOUSE_BUTTON::INVALID)
			: INPUT_BINDING(in_input_type)
			, m_button(in_button)
		{}

		INPUT_BINDING_TYPE get_binding_type() const override { return INPUT_BINDING_TYPE::AXIS; }

		bool operator==(const AXIS_INPUT_BINDING& in_other) const
		{
			return INPUT_BINDING::operator==(in_other) && m_button == in_other.m_button;
		}

		MOUSE_BUTTON m_button{ MOUSE_BUTTON::INVALID };
	};

	struct ACTION_INPUT_BINDING : public INPUT_BINDING
	{
		ACTION_INPUT_BINDING(EINPUT_TYPE in_input_type, KEY in_key, bool m_ctrl_is_down = false,
			bool m_alt_is_down = false, bool m_shift_is_down = false)
			: INPUT_BINDING(in_input_type)
			, m_key(in_key)
		{}

		INPUT_BINDING_TYPE get_binding_type() const override { return INPUT_BINDING_TYPE::ACTION; }

		bool operator==(const ACTION_INPUT_BINDING& in_other) const
		{
			return INPUT_BINDING::operator==(in_other) && m_key == in_other.m_key && m_ctrl_is_down == in_other.m_ctrl_is_down
				&& m_alt_is_down == in_other.m_alt_is_down && m_shift_is_down == in_other.m_shift_is_down;
		}

		KEY m_key{ KEY::INVALID };

		bool m_ctrl_is_down{ false };
		bool m_alt_is_down{ false };
		bool m_shift_is_down{ false };
	};

	struct INPUT_BINDING_WRAPPER
	{
		INPUT_BINDING_WRAPPER(INPUT_BINDING* in_ptr)
			: m_ptr(in_ptr)
		{}

		INPUT_BINDING* m_ptr{ nullptr };

		bool operator==(const INPUT_BINDING_WRAPPER& in_other) const
		{
			return *m_ptr == *in_other.m_ptr;
		}
	};
}

namespace std
{
	template <>
	struct hash<input::INPUT_BINDING>
	{
		size_t operator()(const input::INPUT_BINDING& in_input_binding) const
		{
			// Compute individual hash values for class' data members and combine them using XOR and bit shifting.
			return hash<input::EINPUT_TYPE>()(in_input_binding.m_input_type);;
		}
	};

	template <>
	struct hash<input::AXIS_INPUT_BINDING>
	{
		size_t operator()(const input::AXIS_INPUT_BINDING& in_input_binding) const
		{
			// Compute individual hash values for class' data members and combine them using XOR and bit shifting.
			return ((hash<input::INPUT_BINDING>()(in_input_binding) ^ (hash<input::MOUSE_BUTTON>()(in_input_binding.m_button) << 1)) >> 1);
		}
	};

	template <>
	struct hash<input::ACTION_INPUT_BINDING>
	{
		size_t operator()(const input::ACTION_INPUT_BINDING& in_input_binding) const
		{
			// Compute individual hash values for class' data members and combine them using XOR and bit shifting.
			return ((hash<input::INPUT_BINDING>()(in_input_binding) ^ (hash<input::KEY>()(in_input_binding.m_key) << 1) ^ (hash<bool>()(in_input_binding.m_ctrl_is_down) << 1)
				^ (hash<bool>()(in_input_binding.m_alt_is_down) << 1/*2?*/) ^ (hash<bool>()(in_input_binding.m_shift_is_down) << 1)) >> 1);
		}
	};

	template <>
	struct hash<input::INPUT_BINDING_WRAPPER>
	{
		size_t operator()(const input::INPUT_BINDING_WRAPPER& in_input_binding) const
		{
			// Compute individual hash values for class' data members and combine them using XOR and bit shifting.
			const input::INPUT_BINDING_TYPE binding_type = in_input_binding.m_ptr->get_binding_type();
			switch (binding_type)
			{
			case input::INPUT_BINDING_TYPE::AXIS:
			{
				input::AXIS_INPUT_BINDING* axis_input_binding = dynamic_cast<input::AXIS_INPUT_BINDING*>(in_input_binding.m_ptr);
				return hash<input::AXIS_INPUT_BINDING>()(*axis_input_binding);
			}
			case input::INPUT_BINDING_TYPE::ACTION:
			{
				input::ACTION_INPUT_BINDING* action_input_binding = dynamic_cast<input::ACTION_INPUT_BINDING*>(in_input_binding.m_ptr);
				return hash<input::ACTION_INPUT_BINDING>()(*action_input_binding);
			}
			default:
			{
				ASSERT_SHOULDNT_BE_HERE();
			}
			}

			//uint8_t key_as_uint8 = static_cast<uint8_t>(in_input_binding.m_key);
		}
	};
}
