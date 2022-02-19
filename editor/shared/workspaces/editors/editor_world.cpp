
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_world.h"

#include "engine/shared/game_framework/world.h"

namespace editor
{
	EDITOR_WORLD::EDITOR_WORLD()
	{
		//m_children.emplace_back( new engine::WORLD );
	}

	EDITOR_WORLD::~EDITOR_WORLD() = default;

	bool EDITOR_WORLD::receive_event( const engine::EVENT& in_event )
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

	void EDITOR_WORLD::listen()
	{

	}

	void EDITOR_WORLD::setup_operations()
	{

	}

	void EDITOR_WORLD::setup_widgets()
	{

	}

	void EDITOR_WORLD::setup_keymap()
	{

	}
}
