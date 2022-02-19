
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_assets.h"

#include "editor/shared/workspaces/editors/editor_components/editor_component_files.h"

namespace editor
{
	EDITOR_ASSETS::EDITOR_ASSETS()
	{
		m_children.emplace_back( std::make_unique<EDITOR_COMPONENT_FILES>() );
	}

	EDITOR_ASSETS::~EDITOR_ASSETS() = default;

	bool EDITOR_ASSETS::receive_event( const engine::EVENT& in_event )
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

	void EDITOR_ASSETS::listen()
	{

	}

	void EDITOR_ASSETS::setup_operations()
	{

	}

	void EDITOR_ASSETS::setup_widgets()
	{

	}

	void EDITOR_ASSETS::setup_keymap()
	{

	}
}
