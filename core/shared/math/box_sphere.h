//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/vector3d.h"

namespace core
{
	struct BOX_SPHERE
	{
		core::FVECTOR3D m_centre;

		core::FVECTOR3D m_box_extents;

		float m_sphere_radius{ 0.0f };
	};
}
