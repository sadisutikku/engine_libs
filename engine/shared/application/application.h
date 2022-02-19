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

#include "core/shared/patterns/unique_id.h"
#include "engine/shared/game_framework/game_instance.h"

#include <chrono>
#include <unordered_map>

#if PLATFORM_WINDOWS
#include "engine/platforms/application/windows/application_windows.h"
using APPLICATION_PLATFORM = engine::APPLICATION_WINDOWS;
#endif

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace core
{
	class WORLD;
}

namespace engine
{
	inline constexpr uint32_t MAIN_INSTANCE_ID{ 0u };
	inline constexpr core::UNIQUE_ID POST_APP_ITERATION_EVENT_ID{ 'PAIt' };

	class APPLICATION : public APPLICATION_PLATFORM, public core::NON_AUTOMATIC_SINGLETON<APPLICATION>
	{
		friend class core::NON_AUTOMATIC_SINGLETON<APPLICATION>;

	public:
		static engine::APPLICATION* create_singleton_custom();

		void run_loop();

		//std::chrono::duration<double> get_delta_time() const { return m_delta_time; }

		const GAME_INSTANCE& get_main_game_instance() const { return m_game_instances.at( MAIN_INSTANCE_ID ); }
		GAME_INSTANCE& get_main_game_instance() { return m_game_instances.at( MAIN_INSTANCE_ID ); }

	protected:
		APPLICATION();

		std::unordered_map<uint32_t, GAME_INSTANCE> m_game_instances;
		
		std::chrono::system_clock::time_point m_engine_start_time{};
		std::chrono::system_clock::time_point m_last_time{};
		std::chrono::duration<float> m_delta_time{}; // @todo make a singleton per thread and make this available via each singleton

		bool m_should_shutdown{ false };

		virtual void startup();

		void iteration();

		virtual void shutdown();

		using APPLICATION_PLATFORM::process_os_messages;
	};
}