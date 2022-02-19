


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/buffer_vulkan.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/utilities_vulkan.h"

namespace gui
{
	void BUFFER_VULKAN::_destroy( RENDERING_DEVICE_VULKAN& device_vulkan, VkBuffer* other_buffer )
	{
		VkBuffer* buffer{ other_buffer ? other_buffer : m_buffer };

		// Wait in case it's still in use by the GPU
		//device_vulkan.Queue_WaitAll();

		auto context{ device_vulkan.get_context() };

		if ( m_mapped )
		{
			vmaUnmapMemory( context->allocator, m_allocation );
			m_mapped = nullptr;
		}

		if ( buffer )
		{
			uint64_t allocation_id = reinterpret_cast<uint64_t>(buffer);
			auto it = context->allocations.find( allocation_id );
			if ( it != context->allocations.end() )
			{
				VmaAllocation allocation = it->second;
				vmaDestroyBuffer( context->allocator, *buffer, allocation );
				context->allocations.erase( allocation_id );
				buffer = nullptr;
			}
		}
	}

	bool BUFFER_VULKAN::_create( RENDERING_DEVICE_BASE& device, const BUFFER_TYPE buffer_type, const void* data )
	{
		RENDERING_DEVICE_VULKAN& device_vulkan = static_cast<RENDERING_DEVICE_VULKAN&>(device);

		// Destroy previous buffer
		_destroy( device_vulkan );

		bool use_staging = data != nullptr;
		if ( !use_staging )
		{
			VkMemoryPropertyFlags flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			flags |= !m_persistent_mapping ? VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0;
			VmaAllocation allocation = create_buffer( device_vulkan, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, flags, true, data );
			if ( !allocation )
				return false;

			m_allocation = allocation;
			m_is_mappable = true;
		}
		else
		{
			// The reason we use staging is because memory with VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT is not mappable but it's fast, we want that.

			// Create staging/source buffer and copy the vertices to it
			VkBuffer* staging_buffer{};
			VmaAllocation allocation_staging = create_buffer( device_vulkan, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, false, data, staging_buffer );
			if ( !allocation_staging )
				return false;

			// Create destination buffer
			bool written_frequently = false;
			VmaAllocation allocation = create_buffer( device_vulkan, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, written_frequently, nullptr );
			if ( !allocation )
				return false;

			// Copy staging buffer to destination buffer
			{
				// Create command buffer
				VkCommandBuffer cmd_buffer = command_buffer_immediate::begin( device_vulkan, RHI_Queue_Transfer );

				VkBuffer* buffer_vk = m_buffer;
				VkBuffer* buffer_staging_vk = staging_buffer;

				// Copy
				VkBufferCopy copy_region = {};
				copy_region.size = m_total_size;
				vkCmdCopyBuffer( cmd_buffer, *buffer_staging_vk, *buffer_vk, 1, &copy_region );

				// Flush and free command buffer
				if ( !command_buffer_immediate::end( RHI_Queue_Transfer ) )
					return false;

				// Destroy staging resources
				_destroy( device_vulkan, staging_buffer );
			}

			m_allocation = allocation;
			m_is_mappable = false;
		}

		return true;
	}

	VmaAllocation BUFFER_VULKAN::create_buffer( RENDERING_DEVICE_VULKAN& device_vulkan, VkBufferUsageFlags usage, VkMemoryPropertyFlags memory_property_flags,
												bool written_frequently, const void* data, VkBuffer* other_buffer )
	{
		VkBuffer* buffer{ other_buffer ? other_buffer : m_buffer };

		VmaAllocator allocator = device_vulkan.get_context()->allocator;

		VkBufferCreateInfo buffer_create_info = {};
		buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_create_info.size = m_total_size;
		buffer_create_info.usage = usage;
		buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		bool used_for_staging = (usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) != 0;

		VmaAllocationCreateInfo allocation_create_info = {};
		allocation_create_info.usage = used_for_staging ? VMA_MEMORY_USAGE_CPU_ONLY : (written_frequently ? VMA_MEMORY_USAGE_CPU_TO_GPU : VMA_MEMORY_USAGE_GPU_ONLY);
		allocation_create_info.preferredFlags = memory_property_flags;

		// Create buffer, allocate memory and bind it to the buffer
		VmaAllocation allocation{};
		VmaAllocationInfo allocation_info;
		if ( vmaCreateBuffer( allocator, &buffer_create_info, &allocation_create_info, buffer, &allocation, &allocation_info ) != VK_SUCCESS )
			return nullptr;

		// Keep allocation reference
		device_vulkan.get_context()->allocations[reinterpret_cast<uint64_t>(buffer)] = allocation;

		// If a pointer to the buffer data has been passed, map the buffer and copy over the data
		if ( data != nullptr )
		{
			VkMemoryPropertyFlags memory_flags;
			vmaGetMemoryTypeProperties( allocator, allocation_info.memoryType, &memory_flags );

			bool is_mappable = (memory_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
			bool is_host_coherent = (memory_flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0;

			// Memory in Vulkan doesn't need to be unmapped before using it on GPU, but unless a
			// memory type has VK_MEMORY_PROPERTY_HOST_COHERENT_BIT flag set, you need to manually
			// invalidate cache before reading of mapped pointer and flush cache after writing to
			// mapped pointer. Map/unmap operations don't do that automatically.

			if ( is_mappable )
			{
				if ( !is_host_coherent )
				{
					vmaInvalidateAllocation( allocator, allocation, 0, m_total_size );/* != VK_SUCCESS )*/
						return allocation;
				}

				void* mapped = nullptr;
				if ( vmaMapMemory( allocator, allocation, &mapped ) != VK_SUCCESS )
				{
					memcpy( mapped, data, m_total_size );

					if ( !is_host_coherent )
					{
						vmaFlushAllocation( allocator, allocation, 0, m_total_size );/* != VK_SUCCESS )*/
							return allocation;
					}

					vmaUnmapMemory( allocator, allocation );
				}
			}
			else
			{
				ASSERT_FAILED( "Allocation ended up in non-mappable memory. You need to create CPU-side buffer in VMA_MEMORY_USAGE_CPU_ONLY and make a transfer." );
			}
		}

		return allocation;
	}
}
