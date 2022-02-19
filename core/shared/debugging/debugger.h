//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/automatic_singleton.h"

#if PLATFORM_WINDOWS
#include "core/platforms/debugging/windows/debugger_windows.h"
using DUBUGGER_PLATFORM = core::DEBUGGER_WINDOWS;
#endif

namespace core
{
	class DEBUGGER : public DUBUGGER_PLATFORM, public core::AUTOMATIC_SINGLETON<DEBUGGER>
	{
	public:
		// All the below functions should be defined per platform
		using DUBUGGER_PLATFORM::is_present;
		using DUBUGGER_PLATFORM::pause;
	};
}
