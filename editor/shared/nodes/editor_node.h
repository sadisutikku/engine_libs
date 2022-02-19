
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/nodes/node.h"

#include <memory>
#include <vector>

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace engine
{
	struct EVENT;
}

namespace editor
{
	class EDITOR_BASE;

	class EDITOR_NODE : public engine::NODE
	{
	public:
		EDITOR_NODE();
		~EDITOR_NODE();

		bool receive_event( const engine::EVENT& in_event ) override;

	private:
		void setup_imgui();

		void apply_style();

		bool begin_window();

		void shutdown_imgui();

		std::vector< std::unique_ptr<EDITOR_BASE> > m_active_editors;
	};
}
