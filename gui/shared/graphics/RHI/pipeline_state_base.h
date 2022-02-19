
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/rectangle.h"
#include "gui/shared/graphics/RHI/RHI_types.h"
#include "gui/shared/graphics/RHI/viewport.h"

namespace gui
{
	struct PIPELINE_STATE_BASE
	{
		VIEWPORT m_viewport;
		core::RECTANGLE<uint32_t> m_scissor;
		RHI_PrimitiveTopology_Mode primitive_topology{ RHI_PrimitiveTopology_Unknown };
	};
}
