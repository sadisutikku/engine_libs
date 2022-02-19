
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/buffer_base.h"

#include <gui/thirdparty/vk_mem_alloc.h>
#include <gui/thirdparty/vulkan/vulkan.h>

namespace gui
{
	class RENDERING_DEVICE_VULKAN;

	class BUFFER_VULKAN : public BUFFER_BASE
	{
	public:


	private:
		bool _create( RENDERING_DEVICE_BASE& device, BUFFER_TYPE buffer_type, const void* vertices ) override;

		VmaAllocation create_buffer( RENDERING_DEVICE_VULKAN& device_vulkan, VkBufferUsageFlags usage, VkMemoryPropertyFlags memory_property_flags,
									 bool written_frequently, const void* data, VkBuffer* other_buffer = nullptr );

		void _destroy( RENDERING_DEVICE_VULKAN& device_vulkan, VkBuffer* other_buffer = nullptr );

		VkBuffer* m_buffer{};
		VmaAllocation m_allocation{};
		bool m_is_mappable{ true };

		bool m_persistent_mapping{ true };
		void* m_mapped{};
	};
}
