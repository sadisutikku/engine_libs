
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_components/editor_component_files.h"

#include "editor/shared/ui/widgets/widget_assets.h"

namespace editor
{
	EDITOR_COMPONENT_FILES::EDITOR_COMPONENT_FILES()
	{
		setup_widgets();
	}

	bool EDITOR_COMPONENT_FILES::receive_event( const engine::EVENT& in_event )
	{
		auto handled = EDITOR_COMPONENT_BASE::receive_event( in_event );

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{

			}
			break;
		}

		return handled;
	}

	void EDITOR_COMPONENT_FILES::listen()
	{

	}

	void EDITOR_COMPONENT_FILES::setup_widgets()
	{
		m_children.emplace_back( new WIDGET_ASSETS );
	}
}
