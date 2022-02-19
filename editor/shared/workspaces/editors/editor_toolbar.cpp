
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_toolbar.h"

#include "editor/shared/ui/widgets/widget_toolbar.h"

namespace editor
{
	EDITOR_TOOLBAR::EDITOR_TOOLBAR()
	{
		m_children.emplace_back( new WIDGET_TOOLBAR );
	}

	EDITOR_TOOLBAR::~EDITOR_TOOLBAR() = default;

	bool EDITOR_TOOLBAR::receive_event( const engine::EVENT& in_event )
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

	void EDITOR_TOOLBAR::listen()
	{

	}

	void EDITOR_TOOLBAR::setup_operations()
	{

	}

	void EDITOR_TOOLBAR::setup_widgets()
	{

	}

	void EDITOR_TOOLBAR::setup_keymap()
	{

	}
}
