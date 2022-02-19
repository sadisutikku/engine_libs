
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <string>

namespace engine
{
	class CLIPBOARD
	{
	public:
		static bool		has_text( void* window_handle );
		static bool 	get_text( std::string& text, void* window_handle );
		static void 	set_text( const std::string& text, void* window_handle );

	private:
		CLIPBOARD() = delete;
		~CLIPBOARD() = delete;
	};
}
