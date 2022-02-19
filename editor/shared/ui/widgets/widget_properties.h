
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/ui/widgets/widget.h"

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace engine
{
	class WORLD_OBJECT;
	struct EVENT;
}

namespace editor
{
	class WIDGET_PROPERTIES : public gui::WIDGET
	{
	public:
		WIDGET_PROPERTIES();
		~WIDGET_PROPERTIES();

		bool receive_event( const engine::EVENT& in_event ) override;

	private:
		void render() override;

		engine::WORLD_OBJECT* m_selected_object{};
	};
}
