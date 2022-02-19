//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/platforms/debugging/windows/debugger_windows.h"

#include "core/platforms/include_wrappers/windows/windows.h"
#include "core/shared/logging/log_message.h"

namespace core
{
	bool DEBUGGER_WINDOWS::is_present() const
	{
		return IsDebuggerPresent() != FALSE;
	}

	void DEBUGGER_WINDOWS::pause()
	{
		if (is_present())
		{
			__debugbreak();
		}
	}
}
