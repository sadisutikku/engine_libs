//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/transform.h"

#include "core/shared/math/math_utilities.h"

namespace core
{
    void TRANSFORM::set_position( const FVECTOR3D& position )
    {
        m_local_position = position;
    }

    void TRANSFORM::set_rotation( const QUATERNION& rotation )
    {
        m_local_rotation = rotation;
    }

    void TRANSFORM::set_scale( FVECTOR3D scale )
    {
        scale.x = ( scale.x == 0.0f ) ? EPSILON : scale.x;
        scale.y = ( scale.y == 0.0f ) ? EPSILON : scale.y;
        scale.z = ( scale.z == 0.0f ) ? EPSILON : scale.z;

        m_local_scale = scale;
    }

    void TRANSFORM::move( const FVECTOR3D& local_movement )
    {
        const auto movement = m_local_rotation * local_movement;
        m_local_position += movement;
    }

    void TRANSFORM::rotate( const core::QUATERNION& rotation )
    {
        m_local_rotation *= rotation;
    }

    void TRANSFORM::scale( const core::FVECTOR3D& scale )
    {
        m_local_scale *= scale;
    }

    void TRANSFORM::compute_matrix()
    {
        m_matrix = { m_local_position, m_local_rotation, m_local_scale };
    }
}
