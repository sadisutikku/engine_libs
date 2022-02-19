
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/quaternion.h"

#include "core/shared/math/matrix.h"

namespace core
{
	void QUATERNION::FromAxes( const FVECTOR3D& xAxis, const FVECTOR3D& yAxis, const FVECTOR3D& zAxis )
	{
		*this = MATRIX(
			xAxis.x, yAxis.x, zAxis.x, 0.0f,
			xAxis.y, yAxis.y, zAxis.y, 0.0f,
			xAxis.z, yAxis.z, zAxis.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		).GetRotation();
	}
}
