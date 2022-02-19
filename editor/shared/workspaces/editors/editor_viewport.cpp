
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_viewport.h"

#include "editor/shared/ui/widgets/widget_viewport.h"

namespace editor
{
	EDITOR_VIEWPORT::EDITOR_VIEWPORT()
	{
		m_children.emplace_back( new WIDGET_VIEWPORT );
	}

	EDITOR_VIEWPORT::~EDITOR_VIEWPORT() = default;

	bool EDITOR_VIEWPORT::receive_event( const engine::EVENT& in_event )
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

	void EDITOR_VIEWPORT::listen()
	{

	}

	void EDITOR_VIEWPORT::setup_operations()
	{

	}

	void EDITOR_VIEWPORT::setup_widgets()
	{

	}

	void EDITOR_VIEWPORT::setup_keymap()
	{

	}

	void EDITOR_VIEWPORT::on_world_object_selected( const engine::WORLD_OBJECT* world_object )
	{

	}
}
