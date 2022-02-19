
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_swapchain _base.h"

#include "core/platforms/include_wrappers/windows/windows.h"

#include <array>
#include <gui/thirdparty/vulkan/vulkan.h>

namespace gui
{
	class RENDERING_DEVICE_VULKAN;

	class SWAPCHAIN_VULKAN : public SWAPCHAIN_BASE
	{
	public:
		SWAPCHAIN_VULKAN( HWND window_handle, RENDERING_DEVICE_VULKAN& rhi_device, float width, float height,
						  RHI_Format format, uint32_t flags, const char* name );
		~SWAPCHAIN_VULKAN();

		void present() override;

		bool resize( float width, float height, bool force ) override { return true; }

		uint32_t GetImageIndex() const { return m_image_index; }

		VkImage Get_Resource( uint32_t i = 0 )      const { return m_images[i]; }
		void* Get_Resource_View( uint32_t i = 0 ) const { return m_resource_views[i]; }
		VkSwapchainKHR Get_Resource_View_RenderTarget()  const { return m_swap_chain; }
		void*& GetCmdPool()                           { return m_cmd_pool; }

	private:
		bool create_swapchain();

		void destroy_swapchain();

		bool AcquireNextImage();

		bool m_present_enabled = true;
		uint32_t m_cmd_index = std::numeric_limits<uint32_t>::max();
		uint32_t m_image_index{};
		RENDERING_DEVICE_VULKAN& m_device;
		HWND m_window_handle{};
		void* m_cmd_pool{};
		VkSurfaceKHR m_surface;
		VkSwapchainKHR m_swap_chain;
		//std::array<std::shared_ptr<RHI_Semaphore>, rhi_max_render_target_count> m_image_acquired_semaphore = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		std::array<void*, rhi_max_render_target_count> m_resource_views{};
		std::array<VkImage, rhi_max_render_target_count> m_images{};
	};
}
