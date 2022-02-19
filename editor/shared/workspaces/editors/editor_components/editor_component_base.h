
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/nodes/node.h"

namespace editor
{
	class EDITOR_COMPONENT_BASE : public engine::NODE
	{
	public:
		virtual void listen() = 0;

		virtual void setup_widgets() = 0;
	};
}
