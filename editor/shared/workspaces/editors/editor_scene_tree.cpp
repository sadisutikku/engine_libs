
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_scene_tree.h"

#include "editor/shared/ui/widgets/widget_scene_tree.h"
#include "engine/shared/game_framework/world.h"

namespace editor
{
	EDITOR_SCENE_TREE::EDITOR_SCENE_TREE()
	{
		m_children.emplace_back( new engine::WORLD );
		m_children.emplace_back( new WIDGET_SCENE_TREE );
	}

	EDITOR_SCENE_TREE::~EDITOR_SCENE_TREE() = default;

	bool EDITOR_SCENE_TREE::receive_event( const engine::EVENT& in_event )
	{
		auto handled = EDITOR_BASE::receive_event( in_event );

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{
				
			}
			break;
		}

		return handled;
	}

	void EDITOR_SCENE_TREE::listen()
	{

	}

	void EDITOR_SCENE_TREE::setup_operations()
	{

	}

	void EDITOR_SCENE_TREE::setup_widgets()
	{

	}

	void EDITOR_SCENE_TREE::setup_keymap()
	{

	}
}
