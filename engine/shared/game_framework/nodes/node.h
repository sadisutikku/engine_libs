
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/type_wrappers/vector.h"
#include "engine/shared/events/event_base.h"
#include "engine/shared/events/events_receiver.h"
#include "engine/shared/events/node_events.h"

#include <memory>

//////////////////////////////////////////////////////////////////////
//! Types
//////////////////////////////////////////////////////////////////////

namespace engine
{
	enum class NODE_BRANCH_TYPE : uint8_t
	{
		NONE,
		WORLD_ROOT,
		GRAPHICS_ROOT,
		ENGINE_ROOT,
		EDITOR_ROOT,
	};
}

namespace engine
{
	class NODE : public EVENTS_RECEIVER
	{
	public:
		NODE();
		~NODE();

		bool receive_event( const EVENT& in_event ) override;

		void add_child( NODE* node );

		bool has_children() const { return m_children.empty(); }

		const core::VECTOR< std::unique_ptr<NODE> >& get_children() const { return m_children; }

		NODE_BRANCH_TYPE get_node_type() const { return m_node_type; }

	protected:
		NODE_BRANCH_TYPE m_node_type{ NODE_BRANCH_TYPE::NONE };
		core::VECTOR< std::unique_ptr<NODE> > m_children;
	};
}
