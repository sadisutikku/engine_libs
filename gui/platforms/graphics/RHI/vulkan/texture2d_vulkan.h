
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/texture2d_base.h"

namespace gui
{
	class RENDERER_VULKAN;

	class TEXTURE2D_VULKAN : public TEXTURE2D_BASE
	{
	public:
		TEXTURE2D_VULKAN( RENDERER_VULKAN& renderer, uint32_t width, uint32_t height, RHI_Format format, uint16_t flags, const std::vector<std::byte>& data );
		~TEXTURE2D_VULKAN() = default;

	private:

	};
}
