
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/nodes/node_tree.h"

#include "engine/shared/events/events_manager.h"
#include "engine/shared/events/node_events.h"
#include "engine/shared/game_framework/nodes/node.h"

namespace
{
	engine::NODE* check_children( engine::NODE* node, const engine::NODE_BRANCH_TYPE branch )
	{
		for ( auto& child : node->get_children() )
		{
			if ( child->get_node_type() == branch )
			{
				return child.get();
			}

			if ( engine::NODE* found_node = check_children( child.get(), branch ) )
			{
				return found_node;
			}
		}

		return nullptr;
	}
}

namespace engine
{
	NODE_TREE::NODE_TREE()
		: m_root_node{ std::make_unique<NODE>() }
	{}

	NODE_TREE::~NODE_TREE() = default;

	void NODE_TREE::tick( const float delta_time )
	{
		m_root_node->receive_event( TICK_EVENT{ delta_time } );
	}

	void NODE_TREE::add_node( NODE* node )
	{
		m_root_node->add_child( node );
	}

	bool NODE_TREE::add_node_to_branch( NODE* node, const NODE_BRANCH_TYPE branch )
	{
		if ( branch == NODE_BRANCH_TYPE::NONE )
		{
			add_node( node );
			return true;
		}

		NODE* found_node = check_children( m_root_node.get(), branch );
		if ( found_node )
		{
			EVENTS_MANAGER::get_singleton()->add_event( std::make_unique<ADD_NODE_EVENT>( found_node, node ) );
		}

		return found_node != nullptr;
	}
}
