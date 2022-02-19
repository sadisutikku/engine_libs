


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/texture2d_vulkan.h"

#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
#include "gui/shared/graphics/RHI/RHI_types.h"

#include <vector>

namespace gui
{
	TEXTURE2D_VULKAN::TEXTURE2D_VULKAN( RENDERER_VULKAN& renderer, uint32_t width, uint32_t height, RHI_Format format, uint16_t flags, const std::vector<std::byte>& data )
		: TEXTURE2D_BASE( width, height, format, flags, data )
	{

	}
}
