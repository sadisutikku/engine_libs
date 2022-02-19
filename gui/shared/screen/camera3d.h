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
#include "core/shared/math/transform.h"
#include "core/shared/math/vector2d.h"
#include "core/shared/math/vector3d.h"

namespace gui
{
	class CAMERA3D
	{
	public:
		CAMERA3D();
		~CAMERA3D() = default;

		void move( const core::FVECTOR3D& movement );

		void rotate( const core::FVECTOR2D& delta );

		core::MATRIX get_view_matrix() const { return m_view; }
		core::MATRIX get_projection_matrix() const { return m_projection; }

		float m_near_plane{ 0.3f };
		float m_far_plane{ 1000.0f };

	private:
		core::MATRIX compute_view_matrix() const;

		core::MATRIX compute_projection( bool reverse_z, float near_plane = 0.0f, float far_plane = 0.0f );

		void on_transform_changed();

		core::FVECTOR2D m_mouse_smoothed;
		core::FVECTOR2D m_mouse_rotation;
		core::FVECTOR3D m_local_position;
		core::QUATERNION m_local_rotation;
		core::MATRIX m_view;
		core::MATRIX m_projection;
		core::MATRIX m_view_projection;
		core::TRANSFORM m_transform;
	};
}
