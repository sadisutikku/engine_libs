//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Required classes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/automatic_singleton.h"

#if PLATFORM_WINDOWS
#include "core/platforms/console/windows/windows_console.h"
using CONSOLE_PLATFORM = core::WINDOWS_CONSOLE;
#endif

namespace core
{
	class CONSOLE : public CONSOLE_PLATFORM, public AUTOMATIC_SINGLETON<CONSOLE>
	{
	public:
		using CONSOLE_PLATFORM::is_present;
		using CONSOLE_PLATFORM::open;
		using CONSOLE_PLATFORM::close;
		using CONSOLE_PLATFORM::set_title;

		void write(const std::string& in_line);
	};
}
