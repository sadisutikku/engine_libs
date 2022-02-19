
#pragma once

namespace core
{
	constexpr bool is_platform_windows()
	{
#if PLATFORM_WINDOWS
		return true;
#else
		return false;
#endif
	}
}
