//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "input/shared/mouse/mouse_base.h"

#include "core/platforms/include_wrappers/windows/windows.h"

#include <memory>
#include <stdint.h>

namespace input
{
	class MOUSE_WINDOWS : public MOUSE_BASE
	{
		using super = MOUSE_BASE;

	public:
		bool handle_message(HWND in_window, uint32_t in_message, WPARAM in_wparam, LPARAM in_lparam);

	protected:
		MOUSE_WINDOWS();
		~MOUSE_WINDOWS();

		void on_mouse_button_down(MOUSE_BUTTON in_pressed_button) override;
		void on_mouse_button_up(MOUSE_BUTTON in_released_button) override;

		void on_mouse_wheel_scrolled_vertical( HWND window, WPARAM wparam, LPARAM lparam );

	private:
		void handle_absolute_input( LPARAM lparam );

		bool handle_raw_input();

		struct IMPL;
		std::unique_ptr<IMPL> m_impl;
	};
}