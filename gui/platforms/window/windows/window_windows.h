//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/window/window_base.h"

#include "core/platforms/include_wrappers/windows/windows.h"

#include <string>

namespace gui
{
	class WINDOW_WINDOWS : public WINDOW_BASE
	{
	public:
		WINDOW_WINDOWS();

		HWND find_os_window(const std::string& in_window_name);

		HWND get_native_window() const { return m_window; }

		WINDOW_ID_TYPE get_window_id() const { return reinterpret_cast<WINDOW_ID_TYPE>(m_window); }
		
	protected:
		void show_window();

	private:
		HWND m_window{ nullptr };
	};
}
