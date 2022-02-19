//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h> // todo this should be included by default, so why is there a compiler error!

//////////////////////////////////////////////////////////////////////
//! Constants
//////////////////////////////////////////////////////////////////////

namespace core
{
	using LOG_FLAGS = uint32_t;

	constexpr LOG_FLAGS LOG_NONE = 0;
	constexpr LOG_FLAGS LOG_ENABLED = 1;
	constexpr LOG_FLAGS LOG_ASSERT = 2;
	constexpr LOG_FLAGS LOG_DISPLAY_DIALOG = 4;
	constexpr LOG_FLAGS LOG_SHOW_TIME = 8;
	constexpr LOG_FLAGS LOG_SHOW_NAMES = 16;
	constexpr LOG_FLAGS LOG_STORE_TEXT = 32;
}
