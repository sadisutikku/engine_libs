//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/rectangle.h"
#include "gui/shared/window/window_details.h"

namespace gui
{
	class WINDOW_BASE
	{
	public:
		void set_bounds(const core::FRECTANGLE& in_bounds);
		const core::FRECTANGLE& get_bounds() const;
		float get_width() const;
		float get_height() const;

		WINDOW_ID_TYPE m_window_id{};

	protected:
		WINDOW_BASE() = default;
		virtual ~WINDOW_BASE() = default;

		core::FRECTANGLE m_bounds{ 24.0f, 1.0f, 1063.0f, 807.0f };
	};
}
