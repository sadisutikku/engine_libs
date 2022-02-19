
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/input_layout.h"

namespace gui
{
	class INPUT_LAYOUT_D3D11 : public INPUT_LAYOUT
	{
	private:
		bool _CreateResource( RENDERING_DEVICE_BASE& device, void* vertex_shader_blob ) override;
	};
}
