
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_base.h"

namespace editor
{
	class EDITOR_WORLD : public EDITOR_BASE
	{
	public:
		EDITOR_WORLD();
		~EDITOR_WORLD();

		bool receive_event( const engine::EVENT& in_event ) override;

		void listen() override;

		void setup_operations() override;

		void setup_widgets() override;

		void setup_keymap() override;
	};
}
