//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <memory>

namespace engine
{
	class NODE;
	class NODE_TREE;
	enum class NODE_BRANCH_TYPE : uint8_t;

	class GAME_INSTANCE
	{
	public:
		GAME_INSTANCE();
		~GAME_INSTANCE();

		void tick( float delta_time );

		void add_node_to_root( NODE* node );

		bool add_node_to_branch( NODE* node, NODE_BRANCH_TYPE branch );

		NODE_TREE* get_node_tree() const { return m_node_tree.get(); }

	protected:
		std::unique_ptr<NODE_TREE> m_node_tree;
	};
}