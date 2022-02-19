
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_base.h"

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace engine
{
	class WORLD_OBJECT;
}

namespace editor
{
	class EDITOR_VIEWPORT : public EDITOR_BASE
	{
	public:
		EDITOR_VIEWPORT();
		~EDITOR_VIEWPORT();

		bool receive_event( const engine::EVENT& in_event ) override;

		void listen() override;

		void setup_operations() override;

		void setup_widgets() override;

		void setup_keymap() override;

		void on_world_object_selected( const engine::WORLD_OBJECT* world_object );
	};
}
