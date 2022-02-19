//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <memory>
#include <string>

namespace core
{
	class WINDOWS_CONSOLE
	{
	public:
		WINDOWS_CONSOLE();
		~WINDOWS_CONSOLE();

		bool is_present();
		void open();
		void close();

		void set_title(const std::string& in_title);

		void write(const std::string& in_line);

	private:
		struct impl;
		std::unique_ptr<impl> m_impl{ nullptr };
	};
}
