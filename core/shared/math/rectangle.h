//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/vector4d.h"

#include "core/shared/math/vector2d.h"

namespace core
{
	template<typename TYPE>
	struct RECTANGLE : public VECTOR4D<TYPE>
	{
		using super = VECTOR4D<TYPE>;
		using super::x;
		using super::y;
		using super::z;
		using super::a;

		RECTANGLE() = default;

		RECTANGLE(TYPE InX, TYPE InY, TYPE InZ, TYPE InA)
		{
			x = InX;
			y = InY;
			z = InZ;
			a = InA;
		}

		static const RECTANGLE<TYPE> ZERO_VALUE;

		TYPE get_left() const { return x; }
		TYPE get_right() const { return x + get_width(); }
		TYPE get_width() const { return z; }

		TYPE get_top() const { return y; }
		TYPE get_bottom() const { return y + get_height(); }
		TYPE get_height() const { return a; }

		void set_top_left( const VECTOR2D<TYPE>& position )
		{
			x = position.x;
			y = position.y;
		}

		VECTOR2D<TYPE> get_top_left() const
		{
			return {get_left(), get_top()};
		}

		VECTOR2D<TYPE> get_top_right() const
		{
			return { get_right(), get_top() };
		}

		VECTOR2D<TYPE> get_bottom_left() const
		{
			return { get_left(), get_bottom() };
		}

		VECTOR2D<TYPE> get_bottom_right() const
		{
			return { get_right(), get_bottom() };
		}

		void set_dimensions( const VECTOR2D<TYPE>& in_dimensions)
		{
			z = in_dimensions.x;
			a = in_dimensions.y;
		}

		VECTOR2D<TYPE> get_dimensions() const
		{
			return { get_width(), get_height() };
		}

		template<typename OTHER_TYPE>
		bool is_within( const VECTOR2D<OTHER_TYPE>& point ) const
		{
			return static_cast<TYPE>(point.x) >= get_left() && static_cast<TYPE>(point.y) >= get_top() && static_cast<TYPE>(point.x) <= get_right()
				&& static_cast<TYPE>(point.y) <= get_bottom();
		}
	};

	template<typename TYPE>
	const RECTANGLE<TYPE> RECTANGLE<TYPE>::ZERO_VALUE = RECTANGLE<TYPE>(TYPE(), TYPE());

	using FRECTANGLE = RECTANGLE<float>;
}
