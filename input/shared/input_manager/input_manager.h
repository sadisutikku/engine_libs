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
#include "input/platforms/input_manager/windows/input_manager_windows.h"
using INPUT_MANAGER_PLATFORM = input::INPUT_MANAGER_WINDOWS;
#endif

namespace input
{
	class INPUT_MANAGER : public INPUT_MANAGER_PLATFORM, public core::AUTOMATIC_SINGLETON<INPUT_MANAGER>
	{
	public:
		using INPUT_MANAGER_PLATFORM::handle_platform_message;
	};
}
