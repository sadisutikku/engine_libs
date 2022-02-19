//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/matrix.h"
#include "core/shared/math/quaternion.h"
#include "core/shared/math/vector3d.h"

namespace core
{
    class TRANSFORM
    {
    public:
        void set_position( const FVECTOR3D& position );
        void set_rotation( const QUATERNION& rotation );
        void set_scale( FVECTOR3D scale );

        FVECTOR3D get_position() const { return m_local_position; }
        QUATERNION get_rotation() const { return m_local_rotation; }
        FVECTOR3D get_scale() const { return m_local_scale; }

        void move( const FVECTOR3D& local_movement );
        void rotate( const core::QUATERNION& rotation );
        void scale( const core::FVECTOR3D& scale );

        void compute_matrix();
        const MATRIX& get_matrix() const { return m_matrix; }

    private:
        MATRIX m_matrix;
        FVECTOR3D m_local_position;
        QUATERNION m_local_rotation;
        FVECTOR3D m_local_scale{ FVECTOR3D::ONE_VECTOR };
    };
}
