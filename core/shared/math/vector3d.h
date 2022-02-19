//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <math.h>

namespace core
{
	/**
	 * A vector in 3-D space composed of components (X, Y, Z) with TYPEing point precision.
	 */
	template<typename TYPE>
	struct VECTOR3D
	{
	public:
		VECTOR3D()
		{}

		constexpr explicit VECTOR3D(TYPE InXYZ)
			: x(InXYZ)
			, y(InXYZ)
			, z(InXYZ)
		{}

		constexpr VECTOR3D(TYPE InX, TYPE InY, TYPE InZ)
			: x(InX)
			, y(InY)
			, z(InZ)
		{}

		bool operator==(const VECTOR3D<TYPE>& in_vector) const;

		VECTOR3D<TYPE> operator+(const VECTOR3D<TYPE>& in_vector) const;

		VECTOR3D<TYPE> operator+(float in_scalar) const;

		VECTOR3D<TYPE> operator-(const VECTOR3D<TYPE>& in_vector) const;

		VECTOR3D<TYPE> operator-() const;

		VECTOR3D<TYPE>& operator+=(const VECTOR3D<TYPE>& in_vector);

		VECTOR3D<TYPE>& operator-=(const VECTOR3D<TYPE>& in_vector);

		VECTOR3D<TYPE> operator*(float in_scale) const;

		VECTOR3D<TYPE> operator*( const VECTOR3D<TYPE>& b ) const;

		VECTOR3D<TYPE> operator^(const VECTOR3D<TYPE>& in_vector) const;

		float length_squared() const { return x * x + y * y + z * z; }

		float length() const;

		void normalise();

		VECTOR3D<TYPE> normalised() const;

		static inline VECTOR3D<TYPE> normalised( const VECTOR3D<TYPE>& v1 ) { return v1.normalised(); }

		[[nodiscard]] float Dot( const VECTOR3D<TYPE>& rhs ) const { return x * rhs.x + y * rhs.y + z * rhs.z; }

		static inline float Dot( const VECTOR3D<TYPE>& v1, const VECTOR3D<TYPE>& v2 )    { return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); }

		[[nodiscard]] VECTOR3D<TYPE> Cross( const VECTOR3D<TYPE>& v2 ) const { return Cross( *this, v2 ); }

		static inline VECTOR3D<TYPE> Cross( const VECTOR3D<TYPE>& v1, const VECTOR3D<TYPE>& v2 )
		{
			return {
				v1.y * v2.z - v2.y * v1.z,
				-(v1.x * v2.z - v2.x * v1.z),
				v1.x * v2.y - v2.x * v1.y
			};
		}

		TYPE x{};

		TYPE y{};

		TYPE z{};
	};

	template<typename TYPE>
	inline bool VECTOR3D<TYPE>::operator==(const VECTOR3D<TYPE>& in_vector) const
	{
		return x == in_vector.x && y == in_vector.y && z == in_vector.z;
	}

	template<typename TYPE>
	inline VECTOR3D<TYPE> VECTOR3D<TYPE>::operator+(const VECTOR3D<TYPE>& in_vector) const
	{
		return { x + in_vector.x, y + in_vector.y, z + in_vector.z };
	}

	template<typename TYPE>
	inline VECTOR3D<TYPE> VECTOR3D<TYPE>::operator+(float in_scalar) const
	{
		return { x + in_scalar, y + in_scalar, z + in_scalar };
	}

	template<typename TYPE>
	inline VECTOR3D<TYPE> VECTOR3D<TYPE>::operator-(const VECTOR3D<TYPE>& in_vector) const
	{
		return { x - in_vector.x, y - in_vector.y, z - in_vector.z };
	}

	template<typename TYPE>
	inline VECTOR3D<TYPE> VECTOR3D<TYPE>::operator-() const
	{
		return { -x, -y, -z };
	}

	template<typename TYPE>
	inline VECTOR3D<TYPE>& VECTOR3D<TYPE>::operator+=(const VECTOR3D<TYPE>& in_vector)
	{
		x += in_vector.x;
		y += in_vector.y;
		z += in_vector.z;
		return *this;
	}

	template<typename TYPE>
	inline VECTOR3D<TYPE>& VECTOR3D<TYPE>::operator-=(const VECTOR3D<TYPE>& in_vector)
	{
		x -= in_vector.x;
		y -= in_vector.y;
		z -= in_vector.z;
		return *this;
	}

	template<typename TYPE>
	inline VECTOR3D<TYPE> VECTOR3D<TYPE>::operator*(float in_scale) const
	{
		return { x * in_scale, y * in_scale, z * in_scale };
	}

	template<typename TYPE>
	inline VECTOR3D<TYPE> VECTOR3D<TYPE>::operator*( const VECTOR3D<TYPE>& b ) const
	{
		return { x * b.x, y * b.y, z * b.z };
	}

	// Reverse order operators
	template<typename TYPE>
	inline VECTOR3D<TYPE> operator*( float lhs, const VECTOR3D<TYPE>& rhs ) { return rhs * lhs; }

	template<typename TYPE>
	inline VECTOR3D<TYPE> VECTOR3D<TYPE>::operator^(const VECTOR3D<TYPE>& in_vector) const
	{
		return { y * in_vector.z - z * in_vector.y,
			z * in_vector.x - x * in_vector.z,
			x * in_vector.y - y * in_vector.x };
	}

	template<typename TYPE>
	inline float VECTOR3D<TYPE>::length() const
	{
		return sqrtf( length_squared() );
	}

	template<typename TYPE>
	inline void VECTOR3D<TYPE>::normalise()
	{
		const auto length_sq = length_squared();
		if ( !equals( length_sq, 1.0f ) && length_sq > 0.0f )
		{
			const auto length_inverted = 1.0f / length();
			x *= length_inverted;
			y *= length_inverted;
			z *= length_inverted;
		}
	};

	template<typename TYPE>
	inline VECTOR3D<TYPE> VECTOR3D<TYPE>::normalised() const
	{
		const auto length_sq = length_squared();
		if ( !equals( length_sq, 1.0f ) && length_sq > 0.0f )
		{
			const auto length_inverted = 1.0f / length();
			return (*this) * length_inverted;
		}
		else
		{
			return *this;
		}
	}

	//////////////////////////////////////////////////////////////////////
	//! Types
	//////////////////////////////////////////////////////////////////////

	using FVECTOR3D = VECTOR3D<float>;

	//////////////////////////////////////////////////////////////////////
	//! Constants
	//////////////////////////////////////////////////////////////////////

	static inline constexpr VECTOR3D ZERO_VALUE(0.0f);

	static inline constexpr VECTOR3D ONE_VECTOR(1.0f);

	static inline constexpr VECTOR3D UP_VECTOR(0.0f, 0.0f, 1.0f);

	static inline constexpr VECTOR3D DOWN_VECTOR(0.0f, 0.0f, -1.0f);

	static inline constexpr VECTOR3D FORWARD_VECTOR(1.0f, 0.0f, 0.0f);

	static inline constexpr VECTOR3D BACKWARD_VECTOR(-1.0f, 0.0f, 0.0f);

	static inline constexpr VECTOR3D RIGHT_VECTOR(0.0f, 1.0f, 0.0f);

	static inline constexpr VECTOR3D LEFT_VECTOR(0.0f, -1.0f, 0.0f);
}
