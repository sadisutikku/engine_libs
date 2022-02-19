
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/nodes/node.h"

namespace engine
{
	class ENGINE_NODE : public engine::NODE
	{
	public:
		ENGINE_NODE();
		~ENGINE_NODE();

		bool receive_event( const engine::EVENT& in_event ) override;
	};
}
