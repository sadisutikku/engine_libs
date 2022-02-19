//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "input/shared/input_component.h"

#include "core/shared/debugging/asserts.h"
#include "input/shared/input_types.h"

namespace input
{
	float INPUT_COMPONENT::get_scaled_input(const INPUT_BINDING& in_input_type, const float in_input_value) const
	{
		switch (in_input_type.m_input_type)
		{
		case EINPUT_TYPE::MOUSE_MOVEMENT_HORIZONTAL:
		case EINPUT_TYPE::MOUSE_MOVEMENT_VERTICAL:
		{
			return in_input_value * m_input_yaw_scale;
		}
		default:
		{
			return in_input_value;
		}
		}
	}

	void INPUT_COMPONENT::on_input_received(const INPUT_BINDING_WRAPPER& in_input_type, const float in_input)
	{
		/*auto& itr = m_action_mappings.find(in_input_type);
		if (itr != m_action_mappings.end() )
		{
			const auto& function = itr->second;
			if (function)
			{
				const float scaled_input = get_scaled_input(*in_input_type.m_ptr, in_input);
				function(scaled_input);
			}
		}*/
	}
}
