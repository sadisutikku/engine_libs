//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/non_automatic_singleton.h"

#include "gui/shared/window/window_details.h"

#include <memory>
#include <unordered_map>

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace gui
{
	class WINDOW;
}

namespace gui
{
	class SCREEN_MANAGER : public core::NON_AUTOMATIC_SINGLETON<SCREEN_MANAGER>
	{
		friend class core::NON_AUTOMATIC_SINGLETON<SCREEN_MANAGER>;
		SCREEN_MANAGER();

	public:
		~SCREEN_MANAGER();

		WINDOW_ID_TYPE create_window();

		WINDOW* get_window( WINDOW_ID_TYPE window_id ) const;

		inline static WINDOW_ID_TYPE MAIN_SCREEN_ID{};

	protected:
		void tick();

		std::unordered_map< WINDOW_ID_TYPE, std::unique_ptr<WINDOW> > m_windows;
	};
}
