
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/pipeline_state_base.h"

#include "gui/shared/graphics/RHI/RHI_types.h"
#include "gui/shared/graphics/RHI/viewport.h"

#include <array>
#include <gui/thirdparty/vulkan/vulkan.h>

namespace gui
{
	class RENDERING_DEVICE_VULKAN;
	class SHADER_VULKAN;
	class SWAPCHAIN_VULKAN;
	class TEXTURE2D_VULKAN;

	class PIPELINE_STATE_VULKAN : public PIPELINE_STATE_BASE
	{
	public:
		PIPELINE_STATE_VULKAN( RENDERING_DEVICE_VULKAN& device );
		~PIPELINE_STATE_VULKAN();

		bool create_framebuffer();

		VkFramebuffer GetFrameBuffer() const;

		bool IsGraphics()                               const { return (m_vertex_shader != nullptr || m_pixel_shader != nullptr) && !m_compute_shader; }
		bool IsCompute()                                const { return m_compute_shader != nullptr && !IsGraphics(); }
		bool IsDummy()                                  const { return !m_compute_shader && !m_vertex_shader && !m_pixel_shader; }
		void* GetRenderPass()                           const { return m_render_pass; }

		// RTs
		TEXTURE2D_VULKAN* render_target_depth_texture{};
		std::array<TEXTURE2D_VULKAN*, rhi_max_render_target_count> render_target_color_textures{};

		// RT indices (affect render pass)
		uint32_t render_target_color_texture_array_index{};
		uint32_t render_target_depth_stencil_texture_array_index{};

		// Clear values
		float clear_depth = rhi_depth_load;
		uint32_t clear_stencil = rhi_stencil_load;
		std::array<core::FVECTOR4D, rhi_max_render_target_count> clear_color;
		//==================================================================================

		//= Dynamic, modification is free ============================================
		bool render_target_depth_texture_read_only = false;

		// Constant buffer slots which refer to dynamic buffers (-1 means unused)
		std::array<int32_t, rhi_max_constant_buffer_count> dynamic_constant_buffer_slots =
		{
			0, 1, 2, 3, 4, -1, -1, -1
		};

		bool dynamic_scissor{};
		uint32_t vertex_buffer_stride{};
		RENDERING_DEVICE_VULKAN& m_device;
		SWAPCHAIN_VULKAN* m_render_target_swapchain{};
		SHADER_VULKAN* m_vertex_shader{};
		SHADER_VULKAN* m_pixel_shader{};
		SHADER_VULKAN* m_compute_shader{};
		VkRenderPass m_render_pass{};
		std::array<VkFramebuffer, rhi_max_constant_buffer_count> m_frame_buffers{};

	private:
		void DestroyFrameBuffer();

		uint32_t GetWidth() const;

		uint32_t GetHeight() const;

		void ResetClearValues();
	};
}
