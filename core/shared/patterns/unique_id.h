//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

namespace core
{
	using UNIQUE_ID = uint32_t;
	inline constexpr UNIQUE_ID INVALID_ID{};

	struct SHARED_UNIQUE_ID
	{
		constexpr /*explicit*/ SHARED_UNIQUE_ID( const UNIQUE_ID in_unique_id )
			: unique_id{ in_unique_id }
			, counter{ s_current_count++ }
		{}

		const UNIQUE_ID unique_id{};
		const uint32_t counter{};

		inline static uint32_t s_current_count{};
	};

	//inline constexpr SHARED_UNIQUE_ID INVALID_SHARED_ID{ INVALID_ID };
}
