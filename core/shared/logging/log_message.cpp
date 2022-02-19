//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/logging/log_message.h"

#include "core/shared/logging/log_category.h"

namespace core
{
	LOG_MESSAGE::LOG_MESSAGE(const std::string& in_filename, uint32_t in_line_number, const std::string& in_function_name)
		: m_filename(in_filename)
		, m_line_number(in_line_number)
		, m_function_name(in_function_name)
	{}

	void LOG_MESSAGE::set_category(core::LOG_CATEGORY* in_category)
	{
		m_category = in_category;
	}

	const std::string LOG_MESSAGE::get_full_string() const
	{
		return m_filename + std::to_string(m_line_number) + m_function_name + m_extra_content;
	}
}
