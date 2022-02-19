
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/nodes/engine_node.h"

namespace engine
{
	ENGINE_NODE::ENGINE_NODE()
	{
		m_node_type = engine::NODE_BRANCH_TYPE::ENGINE_ROOT;
	}

	ENGINE_NODE::~ENGINE_NODE() = default;

	bool ENGINE_NODE::receive_event( const engine::EVENT& in_event )
	{
		auto handled{ false };

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{
				handled = NODE::receive_event( in_event );
			}
			break;
		}

		return handled;
	}
}
