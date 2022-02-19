//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#if PLATFORM_WINDOWS
#include "gui/platforms/window/windows/window_windows.h"
using WINDOW_PLATFORM = gui::WINDOW_WINDOWS;
#endif

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace core
{
	template<typename TYPE>
	struct RECTANGLE;
}

namespace gui
{
	class WINDOW : public WINDOW_PLATFORM
	{
	public:
		WINDOW();

		// ensure the following functions exist in each platform implementation
		using WINDOW_PLATFORM::show_window;
		using WINDOW_PLATFORM::get_native_window;
		using WINDOW_PLATFORM::get_window_id;

		bool find_window();
	};
}
