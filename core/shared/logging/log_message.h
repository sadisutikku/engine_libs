//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/logging/log_flags.h"

#include <string>

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace core
{
	class LOG_CATEGORY;
	enum class LOG_VERBOSITY : uint8_t;
}

namespace core
{
	struct LOG_MESSAGE
	{
		LOG_MESSAGE(const std::string& in_filename, uint32_t in_line_number, const std::string& in_function_name);

		void set_category(core::LOG_CATEGORY* in_category);

		void operator += (const std::string& in_extra_content)
		{
			m_extra_content += in_extra_content;
		}

		const std::string get_full_string() const;

	private:
		const std::string m_filename;
		
		const uint32_t m_line_number{ 0 };
		
		const std::string m_function_name;

		std::string m_extra_content;

		core::LOG_CATEGORY* m_category{ nullptr };
	};
}

#define DEBUG_LOG_MESSAGE core::LOG_MESSAGE(__FILE__, static_cast<uint32_t>(__LINE__), __FUNCTION__)
