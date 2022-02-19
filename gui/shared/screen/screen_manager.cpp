//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/screen/screen_manager.h"

#include "gui/shared/window/window.h"
#include "gui/shared/graphics/renderer/renderer.h"

namespace gui
{
	SCREEN_MANAGER::SCREEN_MANAGER() = default;
	
	SCREEN_MANAGER::~SCREEN_MANAGER() = default;

	WINDOW_ID_TYPE SCREEN_MANAGER::create_window()
	{
		auto new_window = std::make_unique<WINDOW>();
		const auto window_id{ new_window->get_window_id() };
		m_windows.try_emplace( window_id, std::move( new_window ) );

		MAIN_SCREEN_ID = window_id; // temp

		RENDERER::get_singleton()->setup_window( window_id );

		return window_id;
	}

	void SCREEN_MANAGER::tick()
	{
		//m_screen->paint();
	}

	WINDOW* SCREEN_MANAGER::get_window( const WINDOW_ID_TYPE window_id ) const
	{
		if ( auto itr = m_windows.find( window_id ); itr != m_windows.end() )
		{
			return itr->second.get();
		}

		//ASSERT_SHOULDNT_BE_HERE();
		return nullptr;
	}
}
