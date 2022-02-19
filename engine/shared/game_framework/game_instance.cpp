
#include "engine/shared/game_framework/game_instance.h"

#include "engine/shared/game_framework/nodes/node_tree.h"
#include "engine/shared/game_framework/nodes/node.h"

namespace engine
{
	GAME_INSTANCE::GAME_INSTANCE()
		: m_node_tree{ std::make_unique<NODE_TREE>() }
	{}

	GAME_INSTANCE::~GAME_INSTANCE() = default;

	void GAME_INSTANCE::tick( const float delta_time )
	{
		m_node_tree->tick( delta_time );
	}

	void GAME_INSTANCE::add_node_to_root( NODE* node )
	{
		m_node_tree->add_node( node );
	}

	bool GAME_INSTANCE::add_node_to_branch( NODE* node, const NODE_BRANCH_TYPE branch )
	{
		return m_node_tree->add_node_to_branch( node, branch );
	}
}
