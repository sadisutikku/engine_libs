

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"

namespace gui
{
	bool RENDERING_CONTEXT_VULKAN::initalise_allocator()
	{
		VmaAllocatorCreateInfo allocator_info = {};
		allocator_info.physicalDevice = device_physical;
		allocator_info.device = device;
		allocator_info.instance = instance;
		allocator_info.vulkanApiVersion = api_version;

		return vmaCreateAllocator( &allocator_info, &allocator ) == VK_SUCCESS;
	}

	void RENDERING_CONTEXT_VULKAN::destroy_allocator()
	{
		if ( allocator != nullptr )
		{
			vmaDestroyAllocator( allocator );
			allocator = nullptr;
		}
	}
}
