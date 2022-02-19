//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "input/shared/input_definition.h"
#include "input/shared/input_forward_declares.h"

namespace input
{
	class INPUT_COMPONENT
	{
		//using MAP_TYPE = std::unordered_map< INPUT_BINDING_WRAPPER, std::function<void(float)> >;

	public:
		float get_scaled_input(const INPUT_BINDING& in_input_type, float in_input_value) const;

		void on_input_received(const INPUT_BINDING_WRAPPER& in_input_type, float in_input);

		void add_action_mapping(const INPUT_BINDING_WRAPPER& in_input_type, std::function<void(float)> in_function)
		{
			//m_action_mappings.emplace(in_input_type, in_function);
		}

		float m_input_pitch_scale{ 100.0f };
		float m_input_yaw_scale{ 100.0f };

	private:
		//MAP_TYPE m_action_mappings;
	};
}