//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/window/window_base.h"

namespace gui
{
	void WINDOW_BASE::set_bounds(const core::FRECTANGLE& in_bounds)
	{
		m_bounds = in_bounds;
	}

	const core::FRECTANGLE& WINDOW_BASE::get_bounds() const
	{
		return m_bounds;
	}

	float WINDOW_BASE::get_width() const
	{
		return m_bounds.get_width();
	}

	float WINDOW_BASE::get_height() const
	{
		return m_bounds.get_height();
	}
}
