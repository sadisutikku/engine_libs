
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_properties.h"

#include "editor/shared/ui/widgets/widget_properties.h"

namespace editor
{
	EDITOR_PROPERTIES::EDITOR_PROPERTIES()
	{
		m_children.emplace_back( std::make_unique<WIDGET_PROPERTIES>() );
	}

	EDITOR_PROPERTIES::~EDITOR_PROPERTIES() = default;

	bool EDITOR_PROPERTIES::receive_event( const engine::EVENT& in_event )
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

	void EDITOR_PROPERTIES::listen()
	{

	}

	void EDITOR_PROPERTIES::setup_operations()
	{

	}

	void EDITOR_PROPERTIES::setup_widgets()
	{

	}

	void EDITOR_PROPERTIES::setup_keymap()
	{

	}
}
