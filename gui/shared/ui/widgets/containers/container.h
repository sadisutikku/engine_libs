//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/ui/widgets/widget.h"

#include <vector>

namespace gui
{
	class CONTAINER : public gui::WIDGET
	{
	public:
		/*void temp();

	protected:
		core::RECTANGLE<int32_t> calculate_desired_size() override;

		void arrange() override;

		void paint(graphics::RENDERER& renderer) override;*/

		std::vector<WIDGET*> m_children;
	};
}
