
#include "engine/platforms/application/windows/application_windows.h"

#include "core/platforms/include_wrappers/windows/windows.h"

namespace engine
{
	APPLICATION_WINDOWS::APPLICATION_WINDOWS()
	{
		
	}

	void APPLICATION_WINDOWS::process_os_messages()
	{
		MSG message;
		while ( PeekMessageW( &message, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
}