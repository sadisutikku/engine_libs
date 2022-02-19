//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/window/windows/window_windows.h"

#include "core/shared/math/rectangle.h"
#include "engine/shared/events/events_manager.h"
#include "gui/shared/screen/screen_manager.h"
#include "gui/thirdparty/imgui/imgui.h"
#include "gui/thirdparty/imgui/windows/imgui_impl_win32.h"

//#include <WtsApi32.h>

namespace
{
	LRESULT CALLBACK handle_windows_message( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
	{
		if ( ImGui_ImplWin32_WndProcHandler( hwnd, message, wparam, lparam ) )
		{
			return true;
		}

		bool handled{ false };

		auto window = gui::SCREEN_MANAGER::get_singleton()->get_window( reinterpret_cast<gui::WINDOW_ID_TYPE>(hwnd) );
		//if ( window )
		{
			for ( size_t index{ 0u }; index < engine::EVENTS_MANAGER::s_event_processors.size() && !handled; ++index )
			{
				handled = engine::EVENTS_MANAGER::s_event_processors[index]( hwnd, message, wparam, lparam );
			}
		}

		return handled ? true : ::DefWindowProcW( hwnd, message, wparam, lparam );
	}

	void register_class()
	{
		HINSTANCE instance = (HINSTANCE)GetModuleHandle( nullptr );

		// Register the Window Class
		WNDCLASSEX wc;
		wc.cbSize = sizeof( WNDCLASSEX );
		wc.style = 0;
		wc.lpfnWndProc = handle_windows_message;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = LoadIcon( instance, IDI_APPLICATION );
		wc.hCursor = LoadCursor( instance, IDC_ARROW );
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = L"ImGui Platform";
		wc.hIconSm = LoadIcon( instance, IDI_APPLICATION );

		if ( !RegisterClassEx( &wc ) )
		{
			MessageBox( nullptr, L"Window registration failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK );
			return;
		}
	}
}

namespace gui
{
	WINDOW_WINDOWS::WINDOW_WINDOWS()
	{
		static bool registered_class{ false };
		if ( !registered_class)
		{
			register_class();
			registered_class = true;
		}

		const core::FRECTANGLE& window_bounds = get_bounds();
		if ( true )
		{
			RECT rect;
			::SetRect( &rect, window_bounds.get_left(), window_bounds.get_top(), window_bounds.get_width(), window_bounds.get_height() );
			::AdjustWindowRect( &rect, 13565952, false );
		}

		m_window = ::CreateWindowW( L"ImGui Platform", L"aaa", 13565952, window_bounds.get_left(), window_bounds.get_top(),
									window_bounds.get_width(), window_bounds.get_height(), nullptr, nullptr, (HINSTANCE)GetModuleHandle( nullptr ), nullptr );
		if ( m_window )
		{
			::SetWindowLongPtr( m_window, 0, (LONG_PTR)this );
			::GdiSetBatchLimit( 1 );
			//::WTSRegisterSessionNotification(m_window, NOTIFY_FOR_THIS_SESSION);
		}

		const HBRUSH__* brush = CreateSolidBrush( RGB( 0, 0, 0 ) );
		SetClassLongPtr( m_window, GCLP_HBRBACKGROUND, (LONG_PTR)brush );

		show_window();
	}

	HWND WINDOW_WINDOWS::find_os_window(const std::string& in_window_name)
	{
		HWND found_window = ::FindWindowEx( 0, 0, 0, LPCWSTR( in_window_name.c_str() ) );
		return found_window;
	}

	void WINDOW_WINDOWS::show_window()
	{
		// Get Windows to completely repaint desktop
		::InvalidateRect(nullptr, nullptr, true);

		::ShowWindow(m_window, SW_SHOWNORMAL);

		::SetForegroundWindow(m_window);
	}
}
