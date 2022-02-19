
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/ui/widgets/widget.h"

namespace editor
{
	class WIDGET_TOOLBAR : public gui::WIDGET
	{
	public:
		WIDGET_TOOLBAR();
		~WIDGET_TOOLBAR() = default;

		bool receive_event( const engine::EVENT& in_event ) override;

	private:
		void render() override;
	};
}
