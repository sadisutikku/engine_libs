
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/rectangle.h"

namespace gui
{
	class VIEWPORT
	{
	public:
		float AspectRatio()    const { return m_area.get_width() / m_area.get_height(); }

		bool is_valid() const { return m_area.is_valid() && m_min_depth >= 0.0f && m_min_depth < m_max_depth && m_max_depth > 0.0f; }

		core::FRECTANGLE m_area;
		float m_min_depth = 0.0f;
		float m_max_depth = 0.0f;
	};
}
