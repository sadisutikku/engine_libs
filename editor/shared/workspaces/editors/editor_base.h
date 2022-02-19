
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/nodes/node.h"

#include <memory>
#include <vector>

namespace editor
{
	class EDITOR_BASE : public engine::NODE
	{
	public:
		virtual void listen() = 0;

		virtual void setup_operations() = 0;

		virtual void setup_widgets() = 0;

		virtual void setup_keymap() = 0;
	};
}
