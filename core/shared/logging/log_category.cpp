//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/logging/log_category.h"

#include "core/shared/debugging/debugger.h"
#include "core/shared/logging/log_manager.h"
#include "core/shared/logging/log_message.h"

namespace core
{
	LOG_CATEGORY::LOG_CATEGORY(const std::string& in_category_name)
		: m_name(in_category_name)
		, m_log_flags(DEFAULT_FLAGS)
		, m_log_verbosity(LOG_VERBOSITY::LOG)
		, m_parent(ROOT_CATEGORY)
	{}

	LOG_CATEGORY::LOG_CATEGORY(const std::string& in_category_name, core::LOG_FLAGS in_log_flags, LOG_VERBOSITY in_log_verbosity)
		: m_name(in_category_name)
		, m_log_flags(in_log_flags)
		, m_log_verbosity(in_log_verbosity)
		, m_parent(ROOT_CATEGORY)
	{}

	LOG_CATEGORY::LOG_CATEGORY(const std::string& in_category_name, core::LOG_FLAGS in_log_flags, LOG_VERBOSITY in_log_verbosity, LOG_CATEGORY in_parent)
		: m_name(in_category_name)
		, m_log_flags(in_log_flags)
		, m_log_verbosity(in_log_verbosity)
		, m_parent(in_parent)
	{}

	void LOG_CATEGORY::LOG_TO_CATEGORY(LOG_CATEGORY& in_category, core::LOG_MESSAGE in_log_message, const std::string& in_message)
	{
		in_log_message += in_message;
		in_log_message.set_category(&in_category);

		in_category.log_message(in_log_message);

		if ( core::equals_bits( in_category.get_log_flags(), core::LOG_ASSERT ) )
		{
			core::DEBUGGER::get_singleton().pause();
		}
	}

	void LOG_CATEGORY::log_message(const core::LOG_MESSAGE& in_log_message)
	{
		core::LOG_MANAGER::get_singleton().log_message(in_log_message);
	}
}
