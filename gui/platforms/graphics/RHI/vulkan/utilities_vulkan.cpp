


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/utilities_vulkan.h"

#include "gui/platforms/graphics/RHI/vulkan/vulkan_types.h"

namespace gui
{
	namespace command_buffer_immediate
	{
		bool cmdbi_object::begin( const RHI_Queue_Type queue_type )
		{
			// Wait
			while ( recording )
			{
				std::this_thread::sleep_for( std::chrono::milliseconds( 16 ) );
			}

			// Initialise
			if ( !initialised )
			{
				// Create command pool
				if ( !command_pool::create( device, cmd_pool, queue_type ) )
					return false;

				// Create command buffer
				if ( !command_buffer::create( device, cmd_pool, cmd_buffer, VK_COMMAND_BUFFER_LEVEL_PRIMARY ) )
					return false;

				initialised = true;
				this->queue_type = queue_type;
			}

			if ( !initialised )
				return false;

			// Begin
			VkCommandBufferBeginInfo begin_info = {};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			if ( vkBeginCommandBuffer( static_cast<VkCommandBuffer>(cmd_buffer), &begin_info ) == VK_SUCCESS )
			{
				recording = true;
			}

			return recording;
		}
	}

	namespace image
	{
		bool set_layout( RENDERING_DEVICE_VULKAN& device_vulkan, void* cmd_buffer, void* image, const VkImageAspectFlags aspect_mask, const uint32_t level_count, const uint32_t layer_count, const RHI_Image_Layout layout_old, const RHI_Image_Layout layout_new )
		{
			VkImageMemoryBarrier image_barrier = {};
			image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			image_barrier.pNext = nullptr;
			image_barrier.oldLayout = vulkan_image_layout[static_cast<uint8_t>(layout_old)];
			image_barrier.newLayout = vulkan_image_layout[static_cast<uint8_t>(layout_new)];
			image_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			image_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			image_barrier.image = static_cast<VkImage>(image);
			image_barrier.subresourceRange.aspectMask = aspect_mask;
			image_barrier.subresourceRange.baseMipLevel = 0;
			image_barrier.subresourceRange.levelCount = level_count;
			image_barrier.subresourceRange.baseArrayLayer = 0;
			image_barrier.subresourceRange.layerCount = layer_count;
			image_barrier.srcAccessMask = layout_to_access_mask( image_barrier.oldLayout, false );
			image_barrier.dstAccessMask = layout_to_access_mask( image_barrier.newLayout, true );

			VkPipelineStageFlags source_stage = 0;
			{
				if ( image_barrier.oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR )
				{
					source_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				}
				else if ( image_barrier.srcAccessMask != 0 )
				{
					source_stage = access_flags_to_pipeline_stage( image_barrier.srcAccessMask, device_vulkan.GetEnabledGraphicsStages() );
				}
				else
				{
					source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				}
			}

			VkPipelineStageFlags destination_stage = 0;
			{
				if ( image_barrier.newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR )
				{
					destination_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				}
				else if ( image_barrier.dstAccessMask != 0 )
				{
					destination_stage = access_flags_to_pipeline_stage( image_barrier.dstAccessMask, device_vulkan.GetEnabledGraphicsStages() );
				}
				else
				{
					destination_stage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				}
			}

			vkCmdPipelineBarrier
			(
				static_cast<VkCommandBuffer>(cmd_buffer),
				source_stage, destination_stage,
				0,
				0, nullptr,
				0, nullptr,
				1, &image_barrier
			);

			return true;
		}
	}
}
