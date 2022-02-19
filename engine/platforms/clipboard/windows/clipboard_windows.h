
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <string>

namespace engine
{
	class CLIPBOARD_WINDOWS
	{
	public:
		static bool		has_text( void* window_handle );
		static bool 	get_text( std::string& text, void* window_handle );
		static void 	set_text( const std::string& text, void* window_handle );
		static void 	set_image( const void* image_pixels, int32_t width, int32_t height );
	};
}
