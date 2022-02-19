
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <array>

namespace core
{
	template<typename TYPE, const uint32_t SIZE>
	class ARRAY : public std::array<TYPE, SIZE>
	{
	public:
		constexpr uint32_t size() const { return SIZE; }
	};
}
