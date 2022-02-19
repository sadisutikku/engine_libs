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

#include "core/shared/patterns/delegate.h"

#if PLATFORM_WINDOWS
#include "input/platforms/mouse/windows/mouse_windows.h"
using MOUSE_PLATFORM = input::MOUSE_WINDOWS;
#endif

namespace input
{
	class MOUSE : public MOUSE_PLATFORM, public core::AUTOMATIC_SINGLETON<MOUSE>
	{
		using super = MOUSE_PLATFORM;

	public:
		using MOUSE_PLATFORM::handle_message;
	};
}