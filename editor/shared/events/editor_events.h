
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/unique_id.h"
#include "engine/shared/events/event_base.h"

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace engine
{
	class WORLD_OBJECT;
}

namespace editor
{
	inline constexpr core::UNIQUE_ID SELECT_WORLD_OBJECT_EVENT_ID{ 'SWOb' };

	struct SELECT_WORLD_OBJECT_EVENT : public engine::EVENT
	{
		SELECT_WORLD_OBJECT_EVENT( engine::WORLD_OBJECT* in_world_object )
			: EVENT{ SELECT_WORLD_OBJECT_EVENT_ID }
			, world_object{ in_world_object }
		{}

		engine::WORLD_OBJECT* world_object;
	};
}
