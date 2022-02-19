

#include "engine/shared/application/application.h"

#include "engine/shared/events/events_manager.h"
#include "engine/shared/game_framework/world.h"

namespace engine
{
	APPLICATION::APPLICATION()
	{
		m_game_instances.try_emplace( MAIN_INSTANCE_ID );

		m_engine_start_time = std::chrono::system_clock::now();
		m_last_time = m_engine_start_time;
	}

	void APPLICATION::run_loop()
	{
		startup();

		while ( !m_should_shutdown )
		{
			iteration();

			EVENTS_MANAGER::get_singleton()->send_event( { POST_APP_ITERATION_EVENT_ID } );
		}

		shutdown();
	}

	void APPLICATION::startup()
	{

	}

	void APPLICATION::iteration()
	{
		const auto current_time = std::chrono::system_clock::now();
		m_delta_time = current_time - m_last_time;
		m_last_time = current_time;

		process_os_messages();

		EVENTS_MANAGER::get_singleton()->process_events();

		for ( auto& [id, game_instance] : m_game_instances )
		{
			game_instance.tick( m_delta_time.count() );
		}
	}

	void APPLICATION::shutdown()
	{

	}
}