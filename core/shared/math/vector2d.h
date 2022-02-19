//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <cmath>
#include <limits>

namespace core
{
	template<typename TYPE>
	struct VECTOR2D
	{
	public:
		VECTOR2D() = default;

		VECTOR2D(TYPE InX, TYPE InY)
			: x(InX)
			, y(InY)
		{}

		float get_size() const
		{
			return sqrtf(x * x + y * y);
		}

		static VECTOR2D direction_between_points(const VECTOR2D& in_first, const VECTOR2D& in_last)
		{
			const VECTOR2D diff(in_first - in_last);
			return /*std::atan2(*/VECTOR2D{ diff.y, diff.x }; // todo normalise
		}

		VECTOR2D operator - (const VECTOR2D& other) const
		{
			return VECTOR2D(x - other.x, y - other.y);
		}

		bool operator == (const VECTOR2D& in_other) const
		{
			return x == in_other.x && y == in_other.y;
		}

		bool operator != (const VECTOR2D& in_other) const
		{
			return !operator==(in_other);
		}

		bool operator < ( const VECTOR2D& in_other ) const
		{
			return x < in_other.x && y < in_other.y;
		}

		bool operator <= ( const VECTOR2D& in_other ) const
		{
			return operator<( in_other ) || operator==( in_other );;
		}

		VECTOR2D operator*( float in_scale ) const
		{
			return { x * in_scale, y * in_scale};
		}

		VECTOR2D operator+( const VECTOR2D& in_vector ) const
		{
			return { x + in_vector.x, y + in_vector.y };
		}

		template<typename OTHER>
		VECTOR2D operator+( const VECTOR2D<OTHER>& in_vector ) const
		{
			return { x + static_cast<TYPE>(in_vector.x), y + static_cast<TYPE>(in_vector.y) };
		}

		VECTOR2D operator+=( const VECTOR2D& in_vector )
		{
			x += in_vector.x;
			y += in_vector.y;
			return *this;
		}

		static const VECTOR2D<TYPE> ZERO_VALUE;

		static const VECTOR2D<TYPE> MAX_VALUE;

		/** VECTOR's X component. */
		TYPE x{};

		/** VECTOR's Y component. */
		TYPE y{};
	};

	template<typename TYPE>
	const VECTOR2D<TYPE> VECTOR2D<TYPE>::ZERO_VALUE = VECTOR2D<TYPE>{ {}, {} };

	template<typename TYPE>
	const VECTOR2D<TYPE> VECTOR2D<TYPE>::MAX_VALUE = VECTOR2D<TYPE>{ std::numeric_limits<TYPE>::max(), std::numeric_limits<TYPE>::max() };
}

#include "core/shared/math/vector_types.h"
