

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/clipboard/clipboard.h"

#if PLATFORM_WINDOWS
#include "engine/platforms/clipboard/windows/clipboard_windows.h"
using CLIPBOARD_PLATFORM = engine::CLIPBOARD_WINDOWS;
#endif

namespace engine
{
	bool CLIPBOARD::has_text( void* window_handle )
	{
		return CLIPBOARD_PLATFORM::has_text( window_handle );
	}

	bool CLIPBOARD::get_text( std::string& text, void* window_handle )
	{
		return CLIPBOARD_PLATFORM::get_text( text, window_handle );
	}

	void CLIPBOARD::set_text( const std::string& text, void* window_handle )
	{
		CLIPBOARD_PLATFORM::set_text( text, window_handle );
	}
}
