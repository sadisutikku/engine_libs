

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/math_utilities.h"
#include "core/shared/math/vector3d.h"

#include <complex>
#include <corecrt_math.h>

namespace core
{
    class QUATERNION
    {
    public:
        QUATERNION()
        {
            x = 0;
            y = 0;
            z = 0;
            w = 1;
        }

        // Constructs a new quaternion with the given components
        QUATERNION(float x, float y, float z, float w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        ~QUATERNION() = default;

        // Creates a new Quaternion from the specified axis and angle.    
        // The angle in radians.
        // The axis of rotation.
        static inline QUATERNION FromAngleAxis(float angle, const FVECTOR3D& axis)
        {
            const float half    = angle * 0.5f;
            const float sin    = sinf(half);
            const float cos    = cosf(half);
            return QUATERNION(axis.x * sin, axis.y * sin, axis.z * sin, cos);
        }

        void FromAxes( const FVECTOR3D& xAxis, const FVECTOR3D& yAxis, const FVECTOR3D& zAxis );

        // Creates a new Quaternion from the specified yaw, pitch and roll angles.
        // Yaw around the y axis in radians.
        // Pitch around the x axis in radians.
        // Roll around the z axis in radians.
        static inline QUATERNION FromYawPitchRoll(float yaw, float pitch, float roll)
        {
            const float halfRoll    = roll * 0.5f;
            const float halfPitch = pitch * 0.5f;
            const float halfYaw    = yaw * 0.5f;

            const float sinRoll    = std::sin(halfRoll);
            const float cosRoll    = std::cos(halfRoll);
            const float sinPitch    = std::sin(halfPitch);
            const float cosPitch    = std::cos(halfPitch);
            const float sinYaw    = std::sin(halfYaw);
            const float cosYaw    = std::cos(halfYaw);

            return QUATERNION(
                cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll,
                sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll,
                cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll,
                cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll
            );
        }
        // Euler angles to quaternion (input in degrees)
        static inline auto FromEulerAngles(const FVECTOR3D& rotation)                              { return FromYawPitchRoll(rotation.y * DEG_TO_RAD, rotation.x * DEG_TO_RAD, rotation.z * DEG_TO_RAD); }
        static inline auto FromEulerAngles(float rotationX, float rotationY, float rotationZ)    { return FromYawPitchRoll(rotationY * DEG_TO_RAD, rotationX * DEG_TO_RAD, rotationZ * DEG_TO_RAD); }

        // Returns Euler angles in degrees
        FVECTOR3D ToEulerAngles() const
        {
            // Derivation from http://www.geometrictools.com/Documentation/EulerAngles.pdf
            // Order of rotations: Z first, then X, then Y
            const float check = 2.0f * (-y * z + w * x);

            if (check < -0.995f)
            {
                return FVECTOR3D
                (
                    -90.0f,
                    0.0f,
                    -atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)) * RAD_TO_DEG
                );
            }

            if (check > 0.995f)
            {
                return FVECTOR3D
                (
                    90.0f,
                    0.0f,
                    atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)) * RAD_TO_DEG
                );
            }

            return FVECTOR3D
            (
                asinf(check) * RAD_TO_DEG,
                atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)) * RAD_TO_DEG,
                atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z)) * RAD_TO_DEG
            );
        }

        // Returns yaw in degrees
        auto Yaw() const    { return ToEulerAngles().y; }
        // Returns pitch in degrees
        auto Pitch() const  { return ToEulerAngles().x; }
        // Returns roll in degrees
        auto Roll() const   { return ToEulerAngles().z; }

        static inline QUATERNION FromToRotation(const FVECTOR3D& start, const FVECTOR3D& end)
        {
            const FVECTOR3D normStart    = start.normalised();
            const FVECTOR3D normEnd   = end.normalised();
            const float d           = normStart.Dot(normEnd);

            if (d > -1.0f + EPSILON)
            {
                const FVECTOR3D c = normStart.Cross(normEnd);
                const float s = sqrtf((1.0f + d) * 2.0f);
                const float invS = 1.0f / s;

                return QUATERNION(
                    c.x * invS,
                    c.y * invS,
                    c.z * invS,
                    0.5f * s);
            }
            else
            {
                FVECTOR3D axis = RIGHT_VECTOR.Cross(normStart);
                if (axis.length() < EPSILON)
                {
                    axis = UP_VECTOR.Cross(normStart);
                }

                return FromAngleAxis(180.0f * DEG_TO_RAD, axis);
            }
        }

        static inline QUATERNION FromLookRotation(const FVECTOR3D& direction, const FVECTOR3D& upDirection = UP_VECTOR)
        {
            QUATERNION ret;
            const FVECTOR3D forward = direction.normalised();

            FVECTOR3D v = forward.Cross(upDirection);
            if (v.length_squared() >= EPSILON)
            {
                v.normalise();
                const FVECTOR3D up = v.Cross(forward);
                const FVECTOR3D right = up.Cross(forward);
                ret.FromAxes(right, up, forward);
            }
            else
                ret = QUATERNION::FromToRotation(FORWARD_VECTOR, forward);

            return ret;
        }

        static inline QUATERNION FromToRotation(const QUATERNION& start, const QUATERNION& end)
        {
            return start.Inverse() * end;
        }

        auto Conjugate() const        { return QUATERNION(-x, -y, -z, w); }
        float LengthSquared() const    { return (x * x) + (y * y) + (z * z) + (w * w); }

        // Normalizes the quaternion
        void Normalize()
        {
            const auto length_squared = LengthSquared();
            if (!equals(length_squared, 1.0f) && length_squared > 0.0f)
            {
                const auto length_inverted = 1.0f / std::sqrt(length_squared);
                x *= length_inverted;
                y *= length_inverted;
                z *= length_inverted;
                w *= length_inverted;
            }
        }

        // Returns a normalized version of the quaternion
        QUATERNION Normalized() const
        {
            const auto length_squared = LengthSquared();
            if (!equals(length_squared, 1.0f) && length_squared > 0.0f)
            {
                const auto length_inverted = 1.0f / std::sqrt(length_squared);
                return (*this) * length_inverted;
            }
            else
                return *this;
        }

        // Returns the inverse
        QUATERNION Inverse() const 
        {
            const float length_squared = LengthSquared();
            if (length_squared == 1.0f)
                return Conjugate();
            else if (length_squared >= EPSILON)
                return Conjugate() * (1.0f / length_squared);
            else
                return Identity;
        }

        QUATERNION& operator =(const QUATERNION& rhs)
        = default;

        static inline QUATERNION Multiply(const QUATERNION& Qa, const QUATERNION& Qb)
        {
            const float x = Qa.x;
            const float y = Qa.y;
            const float z = Qa.z;
            const float w = Qa.w;
            const float num4 = Qb.x;
            const float num3 = Qb.y;
            const float num2 = Qb.z;
            const float num = Qb.w;
            const float num12 = (y * num2) - (z * num3);
            const float num11 = (z * num4) - (x * num2);
            const float num10 = (x * num3) - (y * num4);
            const float num9 = ((x * num4) + (y * num3)) + (z * num2);

            return QUATERNION(
                ((x * num) + (num4 * w)) + num12,
                ((y * num) + (num3 * w)) + num11,
                ((z * num) + (num2 * w)) + num10,
                (w * num) - num9
            );
        }

        QUATERNION operator*(const QUATERNION& rhs) const
        {
            return Multiply(*this, rhs);
        }

        void operator*=(const QUATERNION& rhs)
        {
            *this = Multiply(*this, rhs);
        }

        FVECTOR3D operator*(const FVECTOR3D& rhs) const
        {
            const FVECTOR3D qVec(x, y, z);
            const FVECTOR3D cross1(qVec.Cross(rhs));
            const FVECTOR3D cross2(qVec.Cross(cross1));

            return rhs + 2.0f * (cross1 * w + cross2);
        }

        QUATERNION& operator *=(float rhs)
        {            
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;

            return *this;
        }

        QUATERNION operator *(float rhs) const { return QUATERNION(x * rhs, y * rhs, z * rhs, w * rhs); }

        // Test for equality with a quaternion
        bool operator ==(const QUATERNION& rhs) const
        {
            return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
        }

        // Test for inequality with a quaternion
        bool operator!=(const QUATERNION& rhs) const { return !(*this == rhs); }

        // Test for equality with a quaternion, using epsilon
        bool Equals(const QUATERNION& rhs) const
        {
            return equals(x, rhs.x) && equals(y, rhs.y) && equals(z, rhs.z) && equals(w, rhs.w);
        }

        float x, y, z, w;
        static const QUATERNION Identity;
    };

    inline const QUATERNION QUATERNION::Identity{ 0.0f, 0.0f, 0.0f, 1.0f };

    // Reverse order operators
    //inline FVECTOR3D operator*(const FVECTOR3D& lhs, const Quaternion& rhs) { return rhs * lhs; }
    //inline Quaternion operator*(float lhs, const Quaternion& rhs) { return rhs * lhs; }
}
