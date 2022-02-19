
#pragma once

namespace core
{
	inline constexpr bool is_debug()
	{
#if DEBUG
		return true;
#else
		return false;
#endif
	}

	inline constexpr bool is_editor()
	{
#if EDITOR
		return true;
#else
		return false;
#endif
	}

	inline constexpr bool gui_available()
	{
#if GUI_AVAILABLE
		return true;
#else
		return false;
#endif
	}

	inline constexpr bool input_available()
	{
#if INPUT_AVAILABLE
		return true;
#else
		return false;
#endif
	}
}
