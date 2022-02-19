
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared//patterns/non_automatic_singleton.h"

#include "core/shared/type_wrappers/map.h"
#include "core/shared/type_wrappers/vector.h"
#include "core/shared/patterns/unique_id.h"
#include "engine/shared/events/event_base.h"

#include <functional>
#include <memory>
#include <shared_mutex>

#if PLATFORM_WINDOWS
#include "core/platforms/include_wrappers/windows/windows.h"
#endif

namespace engine
{
	class EVENTS_RECEIVER;

	class EVENTS_MANAGER final : public core::NON_AUTOMATIC_SINGLETON<EVENTS_MANAGER>
	{
		friend class core::NON_AUTOMATIC_SINGLETON<EVENTS_MANAGER>;
		EVENTS_MANAGER() = default;

	public:
		void add_events_receiver( EVENTS_RECEIVER* events_receiver, core::VECTOR<core::UNIQUE_ID> event_ids );

		void remove_events_receiver( EVENTS_RECEIVER* events_receiver );

		//void unregister_for_event( EVENTS_RECEIVER* events_receiver, core::UNIQUE_ID event_ids );

		void add_event( std::unique_ptr<EVENT>&& in_event, uint8_t thread_bitmask = 1u /*THREAD::get_current_thread()*/ );

		void process_events();

		void send_event( const EVENT& in_event );

#if PLATFORM_WINDOWS
		inline static core::VECTOR< std::function<bool( HWND, UINT, WPARAM, LPARAM )> > s_event_processors;
#endif

	private:
		struct THREAD_EVENT
		{
			THREAD_EVENT( const uint8_t in_threads, std::unique_ptr<EVENT>&& in_event )
				: thread_bitmask{ in_threads }
				, shared_event{ std::move( in_event ) }
			{}

			uint8_t thread_bitmask;
			std::unique_ptr<EVENT> shared_event;
		};

		core::VECTOR<THREAD_EVENT> m_events;
		core::MAP< core::UNIQUE_ID, core::VECTOR<EVENTS_RECEIVER*> > m_ids_to_receivers;
		core::MAP< EVENTS_RECEIVER*, core::VECTOR<core::UNIQUE_ID> > m_receiver_to_ids;

		std::shared_mutex m_read_write_mutex;
	};
}
