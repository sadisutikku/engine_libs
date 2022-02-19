

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/platforms/console_application/windows/console_application_windows.h"

#include "core/shared/debugging/asserts.h"
#include "engine/shared/events/events_manager.h"

namespace
{
	BOOL WINAPI console_handler_routine( DWORD message )
	{
		switch ( message )
		{
		case CTRL_CLOSE_EVENT:
			{
				return true;
			}
		}

		return false;
	}
}

namespace engine
{
	void setup()
	{
		[[maybe_unused]] const bool result = SetConsoleCtrlHandler( console_handler_routine, TRUE );
		ASSERT( result, "Call to SetConsoleCtrlHandler failed" );
	}
}
