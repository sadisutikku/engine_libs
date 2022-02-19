
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/nodes/node.h"

namespace engine
{
	NODE::NODE() = default;

	NODE::~NODE() = default;

	bool NODE::receive_event( const EVENT& in_event )
	{
		bool handled{};

		// Pass notification to children
		for ( auto& child : m_children )
		{
			handled |= child->receive_event( in_event );
		}

		switch ( in_event.unique_id )
		{
		case TICK_EVENT_ID:
			{

			}
			break;
		case ENTER_TREE_EVENT_ID:
			{

			}
			break;
		case EXIT_TREE_EVENT_ID:
			{

			}
			break;
		case PRE_DELETE_EVENT_ID:
			{
				m_children.clear();
			}
			break;
		}

		return handled;
	}

	void NODE::add_child( NODE* node )
	{
		m_children.emplace_back( node );
	}
}
