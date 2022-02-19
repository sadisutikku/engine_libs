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
#include "core/shared/misc/bitfields.h"

#include <string>

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace core
{
	struct LOG_MESSAGE;
}

namespace core
{
	enum class LOG_VERBOSITY : uint8_t
	{
		NO_LOGGING = 0,
		LOG,
		ASSERT,
		CRASH,
	};

	class LOG_CATEGORY
	{
	public:
		static constexpr LOG_FLAGS DEFAULT_FLAGS = LOG_DISPLAY_DIALOG | LOG_SHOW_TIME | LOG_SHOW_NAMES | LOG_STORE_TEXT;

		static LOG_CATEGORY ROOT_CATEGORY;
		static LOG_CATEGORY ASSERT_CATEGORY;

		LOG_CATEGORY(const std::string& in_category_name);
		LOG_CATEGORY(const std::string& in_category_name, core::LOG_FLAGS in_log_flags, LOG_VERBOSITY in_log_verbosity);
		LOG_CATEGORY(const std::string& in_category_name, core::LOG_FLAGS in_log_flags, LOG_VERBOSITY in_log_verbosity, LOG_CATEGORY in_parent);
		
		static void LOG_TO_CATEGORY(LOG_CATEGORY& in_category, core::LOG_MESSAGE in_log_message, const std::string& in_message);

		core::LOG_FLAGS get_log_flags() const { return m_log_flags; };

		LOG_VERBOSITY get_log_verbosity() const { return m_log_verbosity; }

	private:
		void log_message(const core::LOG_MESSAGE& in_log_message);

		const std::string m_name;

		core::LOG_FLAGS m_log_flags;

		LOG_VERBOSITY m_log_verbosity;

		LOG_CATEGORY& m_parent;
	};

	inline LOG_CATEGORY LOG_CATEGORY::ROOT_CATEGORY("ROOT");

	inline LOG_CATEGORY LOG_CATEGORY::ASSERT_CATEGORY("ASSERTS", LOG_CATEGORY::DEFAULT_FLAGS | core::LOG_ASSERT, 
		LOG_VERBOSITY::ASSERT);
}
