//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/logging/log_category.h"
#include "core/shared/logging/log_message.h"

//////////////////////////////////////////////////////////////////////
//! Defines
//////////////////////////////////////////////////////////////////////

//#if ENABLE_ASSERTS

#define ASSERT(condition, message) \
	if (!(condition)) { core::LOG_CATEGORY::LOG_TO_CATEGORY(core::LOG_CATEGORY::ASSERT_CATEGORY, DEBUG_LOG_MESSAGE, message); }

#define ASSERT_TRUE(condition) \
	 ASSERT( condition, #condition "expected to be valid." )
//if (!condition) { core::LOG_CATEGORY::LOG_TO_CATEGORY(core::LOG_CATEGORY::ASSERT_CATEGORY, DEBUG_LOG_MESSAGE, "expected to be valid."); }

#define ASSERT_FALSE(condition) \
	if (condition) { core::LOG_CATEGORY::LOG_TO_CATEGORY(core::LOG_CATEGORY::ASSERT_CATEGORY, DEBUG_LOG_MESSAGE, "expected to be invalid."); }

#define ASSERT_FAILED(message) \
	core::LOG_CATEGORY::LOG_TO_CATEGORY(core::LOG_CATEGORY::ASSERT_CATEGORY, DEBUG_LOG_MESSAGE, message);

#define ASSERT_SHOULDNT_BE_HERE() \
	core::LOG_CATEGORY::LOG_TO_CATEGORY(core::LOG_CATEGORY::ASSERT_CATEGORY, DEBUG_LOG_MESSAGE, "expected to be valid.");

#define ASSERT_ONLY_PASS_ONCE() \
	static bool has_passed = false; \
	if (has_passed) { core::LOG_CATEGORY::LOG_TO_CATEGORY(core::LOG_CATEGORY::ASSERT_CATEGORY, DEBUG_LOG_MESSAGE, "Already passed once."); } \
	else { has_passed = true; }

//#else
//
//#define ASSERT(condition, message) UNUSED
//
//#endif
