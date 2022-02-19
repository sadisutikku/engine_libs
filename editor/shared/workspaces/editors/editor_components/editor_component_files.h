
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/workspaces/editors/editor_components/editor_component_base.h"

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace engine
{
	struct EVENT;
}

namespace editor
{
	class EDITOR_COMPONENT_FILES : public EDITOR_COMPONENT_BASE
	{
	public:
		EDITOR_COMPONENT_FILES();
		~EDITOR_COMPONENT_FILES() = default;

		bool receive_event( const engine::EVENT& in_event ) override;

		void listen() override;

		void setup_widgets() override;
	};
}
