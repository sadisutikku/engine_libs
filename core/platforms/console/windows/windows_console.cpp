//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/platforms/console/windows/windows_console.h"

#include "core/platforms/include_wrappers/windows/windows.h"

#include <iostream>

namespace
{
	static bool handle_control_event(unsigned long in_type)
	{
		switch (in_type)
		{
		case CTRL_CLOSE_EVENT:
		case CTRL_C_EVENT:
		{
			// TODO: properly kill engine
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
}

namespace core
{
	struct WINDOWS_CONSOLE::impl
	{
		bool m_is_present{ false };
	};

	WINDOWS_CONSOLE::WINDOWS_CONSOLE()
		: m_impl(std::make_unique<impl>())
	{}

	WINDOWS_CONSOLE::~WINDOWS_CONSOLE() = default;

	bool WINDOWS_CONSOLE::is_present()
	{
		return m_impl->m_is_present;
	}

	void WINDOWS_CONSOLE::open()
	{
		m_impl->m_is_present = ::AllocConsole() != 0;

		SetConsoleCtrlHandler( (PHANDLER_ROUTINE)handle_control_event, true );

		// Redirect CRT standard input, output and error handles to the console window.
		FILE* pNewStdout = nullptr;
		FILE* pNewStderr = nullptr;
		FILE* pNewStdin = nullptr;

		::freopen_s(&pNewStdout, "CONOUT$", "w", stdout);
		::freopen_s(&pNewStderr, "CONOUT$", "w", stderr);
		::freopen_s(&pNewStdin, "CONIN$", "r", stdin);

		// Clear the error state for all of the C++ standard streams. Attempting to accessing the streams before they refer
		// to a valid target causes the stream to enter an error state. Clearing the error state will fix this problem,
		// which seems to occur in newer version of Visual Studio even when the console has not been read from or written
		// to yet.
		std::cout.clear();
		std::cerr.clear();
		std::cin.clear();

		std::wcout.clear();
		std::wcerr.clear();
		std::wcin.clear();
	}

	void WINDOWS_CONSOLE::close()
	{
		if (m_impl->m_is_present)
		{
			m_impl->m_is_present = false;
			::FreeConsole();
		}
	}

	void WINDOWS_CONSOLE::set_title(const std::string& in_title)
	{
		SetConsoleTitle( LPCWSTR( in_title.c_str() ) );
	}

	void WINDOWS_CONSOLE::write(const std::string& in_line)
	{
		std::cout << in_line;

		//DWORD debug = 0;
		//BOOL result = ::WriteConsoleW(::GetStdHandle(STD_OUTPUT_HANDLE), in_line.c_str(), in_line.size(), &debug, nullptr);
	}
}
