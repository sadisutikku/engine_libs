
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/matrix.h"
#include "core/shared/math/vector2d.h"

namespace gui
{
	struct FRAME_CONSTANT_BUFFER
	{
		core::MATRIX view;
		core::MATRIX projection;

		core::MATRIX projection_ortho;
		core::MATRIX view_projection_ortho;

		core::MATRIX view_projection;
	};

	static_assert(sizeof( FRAME_CONSTANT_BUFFER ) % 16 == 0); // d3d: ByteWidth value of D3D11_BUFFER_DESC must be in multiples of 16

	struct GLOBAL_CONSTANT_BUFFER
	{
		core::MATRIX transform;
		core::FVECTOR2D resolution;
		core::FVECTOR2D padding;
	};

	static_assert(sizeof( GLOBAL_CONSTANT_BUFFER ) % 16 == 0); // d3d: ByteWidth value of D3D11_BUFFER_DESC must be in multiples of 16
}
