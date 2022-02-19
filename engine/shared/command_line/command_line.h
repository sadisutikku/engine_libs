//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/non_automatic_singleton.h"

#include <string>
#include <vector>

namespace engine
{
	class COMMAND_LINE : public core::NON_AUTOMATIC_SINGLETON<COMMAND_LINE>
	{
		using super = core::NON_AUTOMATIC_SINGLETON<COMMAND_LINE>;

	public:
		const std::vector<std::string>& get_arguments() const { return m_arguments; }
		
	//private:
		friend class super;
		explicit COMMAND_LINE( std::vector<std::string> in_args );
		~COMMAND_LINE() = default;

		std::vector<std::string> m_arguments;
	};
}
