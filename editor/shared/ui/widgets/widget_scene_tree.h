
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/ui/widgets/widget.h"

#include "core/shared/patterns/delegate.h"

namespace engine
{
	class WORLD_OBJECT;
}

namespace editor
{
	class WIDGET_SCENE_TREE : public gui::WIDGET
	{
	public:
		WIDGET_SCENE_TREE();
		~WIDGET_SCENE_TREE() = default;

		bool receive_event( const engine::EVENT& in_event ) override;

	private:
		void add_children_to_tree( NODE* node );

		void add_node_to_tree( NODE* node );

		void render() override;
	};
}
