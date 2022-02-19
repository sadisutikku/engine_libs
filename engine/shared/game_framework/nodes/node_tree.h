
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <memory>

namespace engine
{
	class NODE;
	enum class NODE_BRANCH_TYPE : uint8_t;

	class NODE_TREE
	{
	public:
		NODE_TREE();
		~NODE_TREE();

		void tick( float delta_time );

		void add_node( NODE* node );

		bool add_node_to_branch( NODE* node, NODE_BRANCH_TYPE branch );

		NODE* get_root_node() const { return m_root_node.get(); }

	private:
		std::unique_ptr<NODE> m_root_node;
	};
}
