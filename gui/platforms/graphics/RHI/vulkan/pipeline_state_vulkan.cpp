


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/pipeline_state_vulkan.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/swapchain_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/texture2d_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/vulkan_types.h"

#include <vector>

namespace
{
	inline VkAttachmentLoadOp get_color_load_op( const core::FVECTOR4D& color )
	{
		if ( color == gui::rhi_color_dont_care )
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		if ( color == gui::rhi_color_load )
			return VK_ATTACHMENT_LOAD_OP_LOAD;

		return VK_ATTACHMENT_LOAD_OP_CLEAR;
	};

	inline VkAttachmentLoadOp get_depth_load_op( const float depth )
	{
		if ( depth == gui::rhi_depth_dont_care )
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		if ( depth == gui::rhi_depth_load )
			return VK_ATTACHMENT_LOAD_OP_LOAD;

		return VK_ATTACHMENT_LOAD_OP_CLEAR;
	};

	inline VkAttachmentLoadOp get_stencil_load_op( const uint32_t stencil )
	{
		if ( stencil == gui::rhi_stencil_dont_care )
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		if ( stencil == gui::rhi_stencil_load )
			return VK_ATTACHMENT_LOAD_OP_LOAD;

		return VK_ATTACHMENT_LOAD_OP_CLEAR;
	};

	/*inline VkAttachmentStoreOp get_stencil_store_op( const RHI_DepthStencilState* depth_stencil_state )
	{
		if ( !depth_stencil_state )
			return VK_ATTACHMENT_STORE_OP_DONT_CARE;

		return depth_stencil_state->GetStencilWriteEnabled() ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
	};*/

	inline bool create_render_pass(
		gui::RENDERING_CONTEXT_VULKAN* rhi_context,
		void* depth_stencil_state,
		gui::SWAPCHAIN_VULKAN* render_target_swapchain,
		std::array<gui::TEXTURE2D_VULKAN*, gui::rhi_max_render_target_count>& render_target_color_textures,
		std::array<core::FVECTOR4D, gui::rhi_max_render_target_count>& render_target_color_clear,
		gui::TEXTURE2D_VULKAN* render_target_depth_texture,
		float clear_value_depth,
		uint32_t clear_value_stencil,
		VkRenderPass& render_pass
	)
	{
		// Attachments
		std::vector<VkAttachmentDescription> attachment_descriptions;
		std::vector<VkAttachmentReference> attachment_references;
		{
			VkAttachmentLoadOp load_op_stencil = get_stencil_load_op( clear_value_stencil );
			VkAttachmentStoreOp store_op_stencil{};// = get_stencil_store_op( depth_stencil_state );

			// Color
			{
				// Swapchain
				if ( render_target_swapchain )
				{
					VkImageLayout layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

					VkAttachmentDescription attachment_desc = {};
					attachment_desc.format = rhi_context->surface_format;
					attachment_desc.samples = VK_SAMPLE_COUNT_1_BIT;
					attachment_desc.loadOp = get_color_load_op( render_target_color_clear[0] );
					attachment_desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
					attachment_desc.stencilLoadOp = load_op_stencil;
					attachment_desc.stencilStoreOp = store_op_stencil;
					attachment_desc.initialLayout = layout;
					attachment_desc.finalLayout = layout;

					// Description
					attachment_descriptions.push_back( attachment_desc );
					// Reference
					attachment_references.push_back( { static_cast<uint32_t>(attachment_references.size()), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );
				}
				else // Textures
				{
					for ( uint32_t i = 0; i < gui::rhi_max_render_target_count; i++ )
					{
						gui::TEXTURE2D_VULKAN* texture = render_target_color_textures[i];
						if ( !texture )
							continue;

						VkImageLayout layout = gui::vulkan_image_layout[static_cast<uint8_t>(texture->GetLayout())];

						VkAttachmentDescription attachment_desc = {};
						attachment_desc.format = gui::vulkan_format[texture->GetFormat()];
						attachment_desc.samples = VK_SAMPLE_COUNT_1_BIT;
						attachment_desc.loadOp = get_color_load_op( render_target_color_clear[i] );
						attachment_desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
						attachment_desc.stencilLoadOp = load_op_stencil;
						attachment_desc.stencilStoreOp = store_op_stencil;
						attachment_desc.initialLayout = layout;
						attachment_desc.finalLayout = layout;

						// Description
						attachment_descriptions.push_back( attachment_desc );
						// Reference
						attachment_references.push_back( { static_cast<uint32_t>(attachment_references.size()), layout } );
					}
				}
			}

			// Depth
			if ( render_target_depth_texture )
			{
				VkImageLayout layout = gui::vulkan_image_layout[static_cast<uint8_t>(render_target_depth_texture->GetLayout())];

				VkAttachmentDescription attachment_desc = {};
				attachment_desc.format = gui::vulkan_format[render_target_depth_texture->GetFormat()];
				attachment_desc.samples = VK_SAMPLE_COUNT_1_BIT;
				attachment_desc.loadOp = get_depth_load_op( clear_value_depth );
				attachment_desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachment_desc.stencilLoadOp = load_op_stencil;
				attachment_desc.stencilStoreOp = store_op_stencil;
				attachment_desc.initialLayout = layout;
				attachment_desc.finalLayout = layout;

				// Description
				attachment_descriptions.push_back( attachment_desc );
				// Reference
				attachment_references.push_back( { static_cast<uint32_t>(attachment_references.size()), layout } );
			}
		}

		// Subpass
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = static_cast<uint32_t>(render_target_depth_texture ? attachment_references.size() - 1 : attachment_references.size());
		subpass.pColorAttachments = attachment_references.data();
		subpass.pDepthStencilAttachment = render_target_depth_texture ? &attachment_references.back() : nullptr;

		VkRenderPassCreateInfo render_pass_info = {};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = static_cast<uint32_t>(attachment_descriptions.size());
		render_pass_info.pAttachments = attachment_descriptions.data();
		render_pass_info.subpassCount = 1;
		render_pass_info.pSubpasses = &subpass;
		render_pass_info.dependencyCount = 0;
		render_pass_info.pDependencies = nullptr;

		return vkCreateRenderPass( rhi_context->device, &render_pass_info, nullptr, &render_pass ) == VK_SUCCESS;
	}

	inline bool create_frame_buffer( gui::RENDERING_CONTEXT_VULKAN* rhi_context, VkRenderPass render_pass, const std::vector<void*>& attachments, const uint32_t width, const uint32_t height, VkFramebuffer& frame_buffer )
	{
		ASSERT_TRUE( width != 0 );
		ASSERT_TRUE( height != 0 );

		VkFramebufferCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		create_info.renderPass = render_pass;
		create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
		create_info.pAttachments = reinterpret_cast<const VkImageView*>(attachments.data());
		create_info.width = width;
		create_info.height = height;
		create_info.layers = 1;

		return vkCreateFramebuffer( rhi_context->device, &create_info, nullptr, &frame_buffer ) == VK_SUCCESS;
	}
}

namespace gui
{
	PIPELINE_STATE_VULKAN::PIPELINE_STATE_VULKAN( RENDERING_DEVICE_VULKAN& device )
		: m_device{ device }
	{}

	PIPELINE_STATE_VULKAN::~PIPELINE_STATE_VULKAN()
	{
		DestroyFrameBuffer();
	}

	bool PIPELINE_STATE_VULKAN::create_framebuffer()
	{
		if ( IsCompute() )
			return true;

		const uint32_t render_target_width = GetWidth();
		const uint32_t render_target_height = GetHeight();

		// Destroy existing frame resources
		DestroyFrameBuffer();

		// Create a render pass
		if ( !create_render_pass( m_device.get_context(), nullptr/*depth_stencil_state*/, m_render_target_swapchain, render_target_color_textures, clear_color, render_target_depth_texture, clear_depth, clear_stencil, m_render_pass ) )
			return false;

		// Name the render pass
		//std::string name = render_target_swapchain ? ("render_pass_swapchain_" + std::to_string( m_hash )) : ("render_pass_texture_" + std::to_string( m_hash ));
		//vulkan_utility::debug::set_name( static_cast<VkRenderPass>(m_render_pass), name.c_str() );

		// Create frame buffer
		if ( m_render_target_swapchain )
		{
			// Create one frame buffer per image
			for ( uint32_t i = 0; i < m_render_target_swapchain->get_buffer_count(); i++ )
			{
				std::vector<void*> attachments = { m_render_target_swapchain->Get_Resource_View( i ) };
				if ( !create_frame_buffer( m_device.get_context(), m_render_pass, attachments, render_target_width, render_target_height, m_frame_buffers[i] ) )
					return false;

				// Name the frame buffer
				//vulkan_utility::debug::set_name( static_cast<VkFramebuffer>(m_frame_buffers[i]), "frame_bufer_swapchain" );
			}

			return true;
		}
		else
		{
			std::vector<void*> attachments;

			// Color
			for ( uint32_t i = 0; i < rhi_max_render_target_count; i++ )
			{
				if ( TEXTURE2D_VULKAN* texture = render_target_color_textures[i] )
				{
					//attachments.emplace_back( texture->Get_Resource_View_RenderTarget( render_target_color_texture_array_index ) );
				}
			}

			// Depth
			if ( render_target_depth_texture )
			{
				//attachments.emplace_back( render_target_depth_texture->Get_Resource_View_DepthStencil( render_target_depth_stencil_texture_array_index ) );
			}

			// Create a frame buffer
			if ( !create_frame_buffer( m_device.get_context(), m_render_pass, attachments, render_target_width, render_target_height, m_frame_buffers[0] ) )
				return false;

			// Name the frame buffer
			//vulkan_utility::debug::set_name( static_cast<VkFramebuffer>(m_frame_buffers[0]), "frame_bufer_texture" );

			return true;
		}
	}

	VkFramebuffer PIPELINE_STATE_VULKAN::GetFrameBuffer() const
	{
		// If this is a swapchain, return the appropriate buffer
		if ( m_render_target_swapchain )
		{
			if ( m_render_target_swapchain->GetImageIndex() >= rhi_max_render_target_count )
			{
				ASSERT_FAILED( "Invalid image index" );
				return nullptr;
			}

			return m_frame_buffers[m_render_target_swapchain->GetImageIndex()];
		}

		// If this is a render texture, return the first buffer 
		return m_frame_buffers[0];
	}

	void PIPELINE_STATE_VULKAN::DestroyFrameBuffer()
	{
		// Wait in case the buffer is still in use by the graphics queue
		m_device.Queue_Wait( RHI_Queue_Graphics );

		for ( uint32_t i = 0; i < rhi_max_render_target_count; i++ )
		{
			if ( void* frame_buffer = m_frame_buffers[i] )
			{
				vkDestroyFramebuffer( m_device.get_context()->device, static_cast<VkFramebuffer>(frame_buffer), nullptr );
			}
		}

		m_frame_buffers.fill( nullptr );

		// Destroy render pass
		vkDestroyRenderPass( m_device.get_context()->device, m_render_pass, nullptr );
		m_render_pass = nullptr;
	}

	uint32_t PIPELINE_STATE_VULKAN::GetWidth() const
	{
		if ( m_render_target_swapchain )
			return (uint32_t)m_render_target_swapchain->get_width();

		if ( render_target_color_textures[0] )
			return render_target_color_textures[0]->get_width();

		if ( render_target_depth_texture )
			return render_target_depth_texture->get_width();

		return 0;
	}

	uint32_t PIPELINE_STATE_VULKAN::GetHeight() const
	{
		if ( m_render_target_swapchain )
			return (uint32_t)m_render_target_swapchain->get_height();

		if ( render_target_color_textures[0] )
			return render_target_color_textures[0]->get_height();

		if ( render_target_depth_texture )
			return render_target_depth_texture->get_height();

		return 0;
	}

	void PIPELINE_STATE_VULKAN::ResetClearValues()
	{
		clear_color.fill( rhi_color_load );
		clear_depth = rhi_depth_load;
		clear_stencil = rhi_stencil_load;
	}
}
