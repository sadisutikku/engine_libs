
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/nodes/node.h"

namespace gui
{
	class GUI_NODE : public engine::NODE
	{
	public:
		GUI_NODE();
		~GUI_NODE();

		bool receive_event( const engine::EVENT& in_event ) override;
	};
}
