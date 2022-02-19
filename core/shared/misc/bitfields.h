//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

namespace core
{
	template<typename TYPE>
	inline constexpr bool equals_bits(TYPE in_value, TYPE in_bitmask) { return (in_value & in_bitmask) == in_bitmask; }

	template<typename TYPE>
	inline constexpr bool contains_bits(TYPE in_value, TYPE in_bitmask) { return (in_value & in_bitmask) != 0; }

	template<typename VALUE_TYPE, typename MASK_TYPE>
	inline constexpr bool contains_bit( VALUE_TYPE in_value, MASK_TYPE in_bitmask ) { return (in_value & in_bitmask) != 0; }

	template<typename TYPE>
	inline constexpr bool are_bits_clear(TYPE in_value, TYPE in_bitmask) { return (in_value & in_bitmask) == 0; }

	template<typename TYPE>
	inline constexpr TYPE set_bits(TYPE in_value, TYPE in_bits) { return static_cast<TYPE>(in_value | in_bits); }

	template<typename TYPE>
	inline constexpr void set_bits(TYPE* in_value, TYPE in_bits) { *in_value |= in_bits; }

	template<typename TYPE>
	inline constexpr TYPE clear_bits(TYPE in_value, TYPE in_bits) { return static_cast<TYPE>(in_value & ~in_bits); }

	template<typename TYPE>
	inline constexpr void clear_bits(TYPE* in_value, TYPE in_bits) { *in_value &= ~in_bits; }
}
