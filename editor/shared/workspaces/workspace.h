
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/type_wrappers/vector.h"
#include "gui/shared/screen/screen_area.h"

namespace editor
{
	struct WORKSPACE
	{
		WORKSPACE();
		~WORKSPACE();

		core::VECTOR<gui::SCREEN_AREA> screen_areas;
	};
}
