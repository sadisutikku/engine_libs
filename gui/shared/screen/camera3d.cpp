//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/screen/camera3d.h"

#include "core/shared/math/math_utilities.h"

#include <algorithm>
#include <cmath>

namespace
{
	constexpr float MOUSE_SENSITIVITY{ 0.02f };
	constexpr float MOUSE_SMOOTHING{ 0.5f };
}

namespace gui
{
	CAMERA3D::CAMERA3D()
	{
		on_transform_changed();
	}

	void CAMERA3D::move( const core::FVECTOR3D& movement )
	{
		m_transform.move( movement );

		on_transform_changed();
	}

	void CAMERA3D::rotate( const core::FVECTOR2D& delta )
	{
		// Get camera rotation.
		m_mouse_rotation.x = m_transform.get_rotation().Yaw();
		m_mouse_rotation.y = m_transform.get_rotation().Pitch();

		const core::FVECTOR2D mouse_delta = delta* MOUSE_SENSITIVITY;//{0.4f, 0.0f};

		//m_mouse_smoothed = core::Lerp( m_mouse_smoothed, mouse_delta, std::clamp( 1.0f - MOUSE_SMOOTHING, 0.0f, 1.0f ) );

		m_mouse_rotation += mouse_delta;// m_mouse_smoothed;

		// Clamp rotation along the x-axis
		m_mouse_rotation.y = std::clamp( m_mouse_rotation.y, -80.0f, 80.0f );

		// Compute rotation
		const core::QUATERNION xQuaternion = core::QUATERNION::FromAngleAxis( m_mouse_rotation.x * core::DEG_TO_RAD, { 0.0f, 1.0f, 0.0f } );
		const core::QUATERNION yQuaternion = core::QUATERNION::FromAngleAxis( m_mouse_rotation.y * core::DEG_TO_RAD, { 1.0f, 0.0f, 0.0f } );
		const core::QUATERNION rotation = xQuaternion * yQuaternion;

		m_transform.set_rotation( rotation );

		on_transform_changed();
	}

	core::MATRIX CAMERA3D::compute_view_matrix() const
	{
		const core::VECTOR3D position{ m_transform.get_position() };
		core::VECTOR3D look_at{ m_transform.get_rotation() * core::VECTOR3D{ 0.0f, 0.0f, 1.0f } };
		core::VECTOR3D up{ m_transform.get_rotation() * core::VECTOR3D{ 0.0f, 1.0f, 0.0f } };

		// offset look_at by current position
		look_at += position;

		return core::MATRIX::CreateLookAtLH( position, look_at, up );
	}

	core::MATRIX CAMERA3D::compute_projection( const bool reverse_z, const float near_plane /*= 0.0f*/, const float far_plane /*= 0.0f*/ )
	{
		float _near = near_plane != 0 ? near_plane : m_near_plane;
		float _far = far_plane != 0 ? far_plane : m_far_plane;

		if ( reverse_z )
		{
			const float temp = _near;
			_near = _far;
			_far = temp;
		}

		//if ( m_projection_type == Projection_Perspective )
		//{
			return core::MATRIX::CreatePerspectiveFieldOfViewLH( 0.899085641f, 2.07257605f, _near, _far );
		/*}
		else if ( m_projection_type == Projection_Orthographic )
		{
			return Matrix::CreateOrthographicLH( GetViewport().width, GetViewport().height, _near, _far );
		}*/

		//return core::MATRIX::Identity;
	}

	void CAMERA3D::on_transform_changed()
	{
		m_view = compute_view_matrix();
		m_projection = compute_projection( true );
		m_view_projection = m_view * m_projection;
	}
}
