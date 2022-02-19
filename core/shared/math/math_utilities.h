#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <limits>

namespace core
{
	constexpr float EPSILON = std::numeric_limits<float>::epsilon();
	constexpr float INFINITY_ = std::numeric_limits<float>::infinity();
	constexpr float PI = 3.14159265359f;
	constexpr float PI_2 = 6.28318530718f;
	constexpr float PI_4 = 12.5663706144f;
	constexpr float PI_DIV_2 = 1.57079632679f;
	constexpr float PI_DIV_4 = 0.78539816339f;
	constexpr float PI_INV = 0.31830988618f;
	constexpr float DEG_TO_RAD = PI / 180.0f;
	constexpr float RAD_TO_DEG = 180.0f / PI;

	inline double Cot( float x )                              { return cos( x ) / sin( x ); }
	inline float CotF( float x )                              { return cosf( x ) / sinf( x ); }
	inline constexpr float DegreesToRadians( const float degrees )   { return degrees * DEG_TO_RAD; }
	inline constexpr float RadiansToDegrees( const float radians )   { return radians * RAD_TO_DEG; }

	// Check for equality but allow for a small error
	template <class T>
	constexpr bool equals( T lhs, T rhs, T error = std::numeric_limits<T>::epsilon() ) { return lhs + error >= rhs && lhs - error <= rhs; }

	template <typename T>
	constexpr int Sign( T x ) { return (static_cast<T>(0) < x) - (x < static_cast<T>(0)); }

	template <class T, class U>
	constexpr T Lerp( T lhs, T rhs, U t ) { return lhs * (static_cast<U>(1) - t) + rhs * t; }

	class Math
	{
	public:
		//static const float PI;
		static const float INV_PI;
		static const float HALF_PI;

		/**
			* Computes the sine and cosine of a scalar value.
			*
			* @param ScalarSin	Pointer to where the Sin result should be stored
			* @param ScalarCos	Pointer to where the Cos result should be stored
			* @param Value  input angles
			*/
		static __forceinline void SinCos( float* ScalarSin, float* ScalarCos, float  Value )
		{
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = (INV_PI * 0.5f) * Value;
			if ( Value >= 0.0f )
			{
				quotient = (float)((int)(quotient + 0.5f));
			}
			else
			{
				quotient = (float)((int)(quotient - 0.5f));
			}
			float y = Value - (2.0f * PI) * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if ( y > HALF_PI )
			{
				y = PI - y;
				sign = -1.0f;
			}
			else if ( y < -HALF_PI )
			{
				y = -PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 11-degree minimax approximation
			*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

			// 10-degree minimax approximation
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			*ScalarCos = sign * p;
		}

		// Conversion Functions

		/**
		 * Converts radians to degrees.
		 * @param	RadVal			Value in radians.
		 * @return					Value in degrees.
		 */
		template<class T>
		static __forceinline auto RadiansToDegrees( T const& RadVal ) -> decltype(RadVal * (180.f / PI))
		{
			return RadVal * (180.f / PI);
		}

		/**
		 * Converts degrees to radians.
		 * @param	DegVal			Value in degrees.
		 * @return					Value in radians.
		 */
		template<class T>
		static __forceinline auto DegreesToRadians( T const& DegVal ) -> decltype(DegVal * (PI / 180.f))
		{
			return DegVal * (PI / 180.f);
		}
	};

	//inline const float Math::PI = 3.1415926535897932f;
	inline const float Math::INV_PI = 0.31830988618f;
	inline const float Math::HALF_PI = 1.57079632679f;
}
