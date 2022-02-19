
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"

#include <array>
#include <atomic>
#include <mutex>
#include <unordered_map>

namespace gui
{
	namespace command_pool
	{
		inline bool create( RENDERING_DEVICE_VULKAN& device_vulkan, void*& cmd_pool, const RHI_Queue_Type queue_type )
		{
			VkCommandPoolCreateInfo cmd_pool_info = {};
			cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			cmd_pool_info.queueFamilyIndex = device_vulkan.Queue_Index( queue_type );
			cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			VkCommandPool* cmd_pool_vk = reinterpret_cast<VkCommandPool*>(&cmd_pool);
			return vkCreateCommandPool( device_vulkan.get_context()->device, &cmd_pool_info, nullptr, cmd_pool_vk ) == VK_SUCCESS;
		}

		inline void destroy( RENDERING_DEVICE_VULKAN& device_vulkan, void*& cmd_pool )
		{
			VkCommandPool cmd_pool_vk = static_cast<VkCommandPool>(cmd_pool);
			vkDestroyCommandPool( device_vulkan.get_context()->device, cmd_pool_vk, nullptr );
			cmd_pool = nullptr;
		}
	}

	namespace command_buffer
	{
		inline bool create( RENDERING_DEVICE_VULKAN& device_vulkan, void*& cmd_pool, void*& cmd_buffer, const VkCommandBufferLevel level )
		{
			VkCommandPool cmd_pool_vk = static_cast<VkCommandPool>(cmd_pool);
			VkCommandBuffer* cmd_buffer_vk = reinterpret_cast<VkCommandBuffer*>(&cmd_buffer);

			VkCommandBufferAllocateInfo allocate_info = {};
			allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocate_info.commandPool = cmd_pool_vk;
			allocate_info.level = level;
			allocate_info.commandBufferCount = 1;

			return vkAllocateCommandBuffers( device_vulkan.get_context()->device, &allocate_info, cmd_buffer_vk ) == VK_SUCCESS;
		}

		inline void destroy( RENDERING_DEVICE_VULKAN& device_vulkan, void*& cmd_pool, void*& cmd_buffer )
		{
			VkCommandPool cmd_pool_vk = static_cast<VkCommandPool>(cmd_pool);
			VkCommandBuffer* cmd_buffer_vk = reinterpret_cast<VkCommandBuffer*>(&cmd_buffer);
			vkFreeCommandBuffers( device_vulkan.get_context()->device, cmd_pool_vk, 1, cmd_buffer_vk );
		}
	}

	namespace command_buffer_immediate
	{
		struct cmdbi_object
		{
			cmdbi_object( RENDERING_DEVICE_VULKAN& device_vulkan )
				: device{ device_vulkan }
			{}

			~cmdbi_object()
			{
				command_buffer::destroy( device, cmd_pool, cmd_buffer );
				command_pool::destroy( device, cmd_pool );
			}

			bool begin( const RHI_Queue_Type queue_type );

			bool submit( const uint32_t wait_flags )
			{
				if ( !initialised )
				{
					ASSERT_FAILED( "Can't submit as the command buffer failed to initialise" );
					return false;
				}

				if ( !recording )
				{
					ASSERT_FAILED( "Can't submit as the command buffer didn't record anything" );
					return false;
				}

				if ( vkEndCommandBuffer( static_cast<VkCommandBuffer>(cmd_buffer) ) != VK_SUCCESS )
				{
					ASSERT_FAILED( "Failed to end command buffer" );
					return false;
				}

				if ( !device.Queue_Submit( queue_type, wait_flags, cmd_buffer, nullptr, nullptr, nullptr ) )
				{
					ASSERT_FAILED( "Failed to submit to queue" );
					return false;
				}

				if ( !device.Queue_Wait( queue_type ) )
				{
					ASSERT_FAILED( "Failed to wait for queue" );
					return false;
				}

				recording = false;
				return true;
			}

			RENDERING_DEVICE_VULKAN& device;
			void* cmd_pool = nullptr;
			void* cmd_buffer = nullptr;
			RHI_Queue_Type queue_type = RHI_Queue_Undefined;
			std::atomic<bool> initialised = false;
			std::atomic<bool> recording = false;
		};

		static std::mutex m_mutex_begin;
		static std::mutex m_mutex_end;
		static std::unordered_map<RHI_Queue_Type, cmdbi_object> m_objects;

		static VkCommandBuffer begin( RENDERING_DEVICE_VULKAN& device_vulkan, const RHI_Queue_Type queue_type )
		{
			std::lock_guard<std::mutex> lock( m_mutex_begin );

			auto [itr, success] = m_objects.try_emplace( queue_type, device_vulkan );

			if ( !itr->second.begin( queue_type ) )
				return nullptr;

			return static_cast<VkCommandBuffer>(itr->second.cmd_buffer);
		}

		static bool end( const RHI_Queue_Type queue_type )
		{
			uint32_t wait_flags;
			if ( queue_type == RHI_Queue_Graphics )
			{
				wait_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			}
			else if ( queue_type == RHI_Queue_Transfer )
			{
				wait_flags = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}

			std::lock_guard<std::mutex> lock( m_mutex_end );
			return m_objects.at(queue_type).submit( wait_flags );
		}
	}

	namespace image
	{
		inline VkPipelineStageFlags access_flags_to_pipeline_stage( VkAccessFlags access_flags, const VkPipelineStageFlags enabled_graphics_shader_stages )
		{
			VkPipelineStageFlags stages = 0;

			while ( access_flags != 0 )
			{
				VkAccessFlagBits AccessFlag = static_cast<VkAccessFlagBits>(access_flags & (~(access_flags - 1)));
				//ASSERT_FAILED( AccessFlag != 0 && (AccessFlag & (AccessFlag - 1)) == 0 );
				access_flags &= ~AccessFlag;

				switch ( AccessFlag )
				{
				case VK_ACCESS_INDIRECT_COMMAND_READ_BIT:
					stages |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
					break;

				case VK_ACCESS_INDEX_READ_BIT:
					stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
					break;

				case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT:
					stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
					break;

				case VK_ACCESS_UNIFORM_READ_BIT:
					stages |= enabled_graphics_shader_stages | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
					break;

				case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT:
					stages |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					break;

				case VK_ACCESS_SHADER_READ_BIT:
					stages |= enabled_graphics_shader_stages | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
					break;

				case VK_ACCESS_SHADER_WRITE_BIT:
					stages |= enabled_graphics_shader_stages | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
					break;

				case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT:
					stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					break;

				case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT:
					stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					break;

				case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT:
					stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
					break;

				case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT:
					stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
					break;

				case VK_ACCESS_TRANSFER_READ_BIT:
					stages |= VK_PIPELINE_STAGE_TRANSFER_BIT;
					break;

				case VK_ACCESS_TRANSFER_WRITE_BIT:
					stages |= VK_PIPELINE_STAGE_TRANSFER_BIT;
					break;

				case VK_ACCESS_HOST_READ_BIT:
					stages |= VK_PIPELINE_STAGE_HOST_BIT;
					break;

				case VK_ACCESS_HOST_WRITE_BIT:
					stages |= VK_PIPELINE_STAGE_HOST_BIT;
					break;

				case VK_ACCESS_MEMORY_READ_BIT:
					break;

				case VK_ACCESS_MEMORY_WRITE_BIT:
					break;

				default:
					ASSERT_FAILED( "Unknown memory access flag" );
					break;
				}
			}
			return stages;
		}

		inline VkPipelineStageFlags layout_to_access_mask( const VkImageLayout layout, const bool is_destination_mask )
		{
			VkPipelineStageFlags access_mask = 0;

			switch ( layout )
			{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				if ( is_destination_mask )
				{
					ASSERT_FAILED( "The new layout used in a transition must not be VK_IMAGE_LAYOUT_UNDEFINED." );
				}
				break;

			case VK_IMAGE_LAYOUT_GENERAL:
				access_mask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				access_mask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
				access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				access_mask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				access_mask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				access_mask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				if ( !is_destination_mask )
				{
					access_mask = VK_ACCESS_HOST_WRITE_BIT;
				}
				else
				{
					ASSERT_FAILED( "The new layout used in a transition must not be VK_IMAGE_LAYOUT_PREINITIALIZED." );
				}
				break;

			case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
				access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
				access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
				access_mask = VK_ACCESS_MEMORY_READ_BIT;
				break;

			default:
				ASSERT_FAILED( "Unexpected image layout" );
				break;
			}

			return access_mask;
		}

		bool set_layout( RENDERING_DEVICE_VULKAN& device_vulkan, void* cmd_buffer, void* image, VkImageAspectFlags aspect_mask, uint32_t level_count, uint32_t layer_count, RHI_Image_Layout layout_old, RHI_Image_Layout layout_new );

		namespace view
		{
			inline bool create( RENDERING_DEVICE_VULKAN& device_vulkan, void* image, void*& image_view, VkImageViewType type, const VkFormat format, const VkImageAspectFlags aspect_mask, const uint32_t level_count = 1, const uint32_t layer_index = 0, const uint32_t layer_count = 1 )
			{
				VkImageViewCreateInfo create_info = {};
				create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				create_info.image = static_cast<VkImage>(image);
				create_info.viewType = type;
				create_info.format = format;
				create_info.subresourceRange.aspectMask = aspect_mask;
				create_info.subresourceRange.baseMipLevel = 0;
				create_info.subresourceRange.levelCount = level_count;
				create_info.subresourceRange.baseArrayLayer = layer_index;
				create_info.subresourceRange.layerCount = layer_count;
				create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				VkImageView* image_view_vk = reinterpret_cast<VkImageView*>(&image_view);
				return vkCreateImageView( device_vulkan.get_context()->device, &create_info, nullptr, image_view_vk ) == VK_SUCCESS;
			}

			//inline bool create( void* image, void*& image_view, const RHI_Texture* texture, const uint32_t array_index = 0, const uint32_t array_length = 1, const bool only_depth = false, const bool only_stencil = false )
			//{
			//	VkImageViewType type = VK_IMAGE_VIEW_TYPE_MAX_ENUM;

			//	if ( texture->GetResourceType() == ResourceType::Texture2d )
			//	{
			//		type = (texture->GetArraySize() == 1) ? VK_IMAGE_VIEW_TYPE_2D : VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			//	}
			//	else if ( texture->GetResourceType() == ResourceType::TextureCube )
			//	{
			//		type = VK_IMAGE_VIEW_TYPE_CUBE;
			//	}

			//	return create( image, image_view, type, vulkan_format[texture->GetFormat()], get_aspect_mask( texture, only_depth, only_stencil ), texture->GetMipCount(), array_index, array_length );
			//}

			inline void destroy( RENDERING_DEVICE_VULKAN& device_vulkan, void*& image_view )
			{
				if ( !image_view )
					return;

				vkDestroyImageView( device_vulkan.get_context()->device, static_cast<VkImageView>(image_view), nullptr );
				image_view = nullptr;
			}

			inline void destroy( RENDERING_DEVICE_VULKAN& device_vulkan, std::array<void*, rhi_max_render_target_count>& image_views )
			{
				for ( void*& image_view : image_views )
				{
					if ( image_view )
					{
						vkDestroyImageView( device_vulkan.get_context()->device, static_cast<VkImageView>(image_view), nullptr );
					}
				}
				image_views.fill( nullptr );
			}
		}
	}
}
