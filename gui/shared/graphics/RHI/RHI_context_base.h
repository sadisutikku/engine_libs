
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_types.h"

namespace gui
{
	struct RENDERING_CONTEXT_BASE
	{
		// Device limits
		uint32_t texture_2d_dimension_max = 16384;
		static constexpr uint8_t descriptors_max = 255;

		// Debugging
#ifdef DEBUG
		bool debug = true;
		bool markers = true;
		bool profiler = true;
#else
		bool debug = false;
		bool markers = false;
		bool profiler = true;
#endif
	};
}
