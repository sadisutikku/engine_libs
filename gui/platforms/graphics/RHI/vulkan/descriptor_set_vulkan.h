
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <gui/thirdparty/vulkan/vulkan.h>

namespace gui
{
	class DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN;
	class RENDERING_DEVICE_VULKAN;

	class DESCRIPTOR_SET_VULKAN
	{
	public:
		DESCRIPTOR_SET_VULKAN( RENDERING_DEVICE_VULKAN& device_vulkan );
		~DESCRIPTOR_SET_VULKAN();

	private:
		DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN* m_descriptor_set_layout_cache{};
		VkDescriptorSet m_descriptor_set{};
	};
}
