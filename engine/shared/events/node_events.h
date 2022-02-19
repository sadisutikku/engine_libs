
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
	class NODE;
}

namespace engine
{
	inline constexpr core::UNIQUE_ID TICK_EVENT_ID{ 'Tick' };
	inline constexpr core::UNIQUE_ID PRE_DELETE_EVENT_ID{ 'PrDe' };
	inline constexpr core::UNIQUE_ID ENTER_TREE_EVENT_ID{ 'EnTr' };
	inline constexpr core::UNIQUE_ID EXIT_TREE_EVENT_ID{ 'ExTr' };
	inline constexpr core::UNIQUE_ID ADD_NODE_EVENT_ID{ 'AdNo' };

	struct TICK_EVENT : public EVENT
    {
        TICK_EVENT( const float in_delta_time )
            : EVENT{ TICK_EVENT_ID }
            , delta_time( in_delta_time )
        {
        }

        ~TICK_EVENT() = default;

        const float delta_time;
    };

	struct ADD_NODE_EVENT : public EVENT
	{
		ADD_NODE_EVENT( NODE* in_parent, NODE* in_child )
			: EVENT{ ADD_NODE_EVENT_ID }
			, parent{ in_parent }
			, child{ in_child }
		{}

		~ADD_NODE_EVENT() = default;

		NODE* parent{};
		NODE* child{};
	};
}
