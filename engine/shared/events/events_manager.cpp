
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/events/events_manager.h"

#include "core/shared/misc/bitfields.h"
#include "engine/shared/events/events_receiver.h"

#include <mutex>

namespace engine
{
	void EVENTS_MANAGER::add_events_receiver( EVENTS_RECEIVER* events_receiver, core::VECTOR<core::UNIQUE_ID> event_ids )
	{
		std::unique_lock unique_lock{ m_read_write_mutex };

		m_receiver_to_ids.emplace( events_receiver, event_ids );

		for ( auto unique_id : event_ids )
		{
			auto& receivers = m_ids_to_receivers[unique_id];
			receivers.push_back( events_receiver );
		}
	}

	void EVENTS_MANAGER::remove_events_receiver( EVENTS_RECEIVER* events_receiver )
	{
		std::unique_lock unique_lock{ m_read_write_mutex };

		if ( auto unique_ids_itr = m_receiver_to_ids.find( events_receiver ); unique_ids_itr != m_receiver_to_ids.end() )
		{
			for ( auto unique_id : unique_ids_itr->second )
			{
				if ( auto receiver_vector_itr = m_ids_to_receivers.find( unique_id ); receiver_vector_itr != m_ids_to_receivers.end() )
				{
					auto receiver_vector = receiver_vector_itr->second;
					if ( auto receiver_itr = std::find( receiver_vector.begin(), receiver_vector.end(), events_receiver ); receiver_itr != receiver_vector.end() )
					{
						receiver_vector_itr->second.erase( receiver_itr );
					}
				}
			}
		}
	}

	void EVENTS_MANAGER::add_event( std::unique_ptr<EVENT>&& in_event, const uint8_t thread_bitmask )
	{
		std::unique_lock unique_lock{ m_read_write_mutex };

		m_events.emplace_back( thread_bitmask, std::move( in_event ) );
	}

	void EVENTS_MANAGER::process_events()
	{
		if ( m_events.empty() )
		{
			return;
		}

		std::shared_lock lock{ m_read_write_mutex };

		const uint8_t current_thread_id{ 1u };

		for ( auto& event : m_events )
		{
			if ( current_thread_id & event.thread_bitmask )
			{
				if ( auto itr = m_ids_to_receivers.find( event.shared_event->unique_id ); itr != m_ids_to_receivers.end() )
				{
					for ( auto receiver : itr->second )
					{
						receiver->receive_event( *event.shared_event );
					}
				}

				core::clear_bits(&event.thread_bitmask, current_thread_id);
			}
		}

		m_events.erase( std::remove_if( m_events.begin(), m_events.end(), []( const THREAD_EVENT& event )
						{
							return event.thread_bitmask == 0u;
						} ) );
	}

	void EVENTS_MANAGER::send_event( const EVENT& in_event )
	{
		std::shared_lock lock{ m_read_write_mutex };

		if ( auto itr = m_ids_to_receivers.find( in_event.unique_id ); itr != m_ids_to_receivers.end() )
		{
			for ( auto receiver : itr->second )
			{
				receiver->receive_event( in_event );
			}
		}
	}
}
