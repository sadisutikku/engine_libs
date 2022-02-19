

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/workspace.h"

#include "engine/shared/game_framework/world.h"

namespace editor
{
	WORKSPACE::WORKSPACE()
	{
		//screen_areas.emplace_back( core::FRECTANGLE{ 0.0f, 0.0f, 800.0f, 600.0f }, engine::WORLD::get_world(), nullptr );
	}

	WORKSPACE::~WORKSPACE() = default;
}
