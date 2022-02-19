
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_context_base.h"

#if PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#define NOMINMAX
#endif

#pragma warning(push, 0) // Hide warnings which belong to Vulkan
#include <gui/thirdparty/vulkan/vulkan.h>
#pragma warning(pop)
#include "gui/thirdparty/vk_mem_alloc.h"

#include <unordered_map>
#include <vector>

namespace gui
{
	struct RENDERING_CONTEXT_VULKAN : public RENDERING_CONTEXT_BASE
	{
        RHI_Api_Type api_type{ RHI_Api_Type::RHI_Api_Vulkan };
        uint32_t api_version = 0;
        VkInstance instance = nullptr;
        VkPhysicalDevice device_physical = nullptr;
        VkDevice device = nullptr;
        VkPhysicalDeviceProperties device_properties = {};
        VkPhysicalDeviceVulkan12Features device_features_1_2 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
        VkPhysicalDeviceFeatures2 device_features = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, &device_features_1_2 };
        VkFormat surface_format = VK_FORMAT_UNDEFINED;
        VkColorSpaceKHR surface_color_space = VK_COLOR_SPACE_MAX_ENUM_KHR;
        VmaAllocator allocator = nullptr;
        std::unordered_map<uint64_t, VmaAllocation> allocations;

        // Extensions
#ifdef DEBUG
        /*
        https://vulkan.lunarg.com/doc/view/1.2.135.0/mac/validation_layers.html

        VK_LAYER_KHRONOS_validation
        ===================================
        The main, comprehensive Khronos validation layer -- this layer encompasses the entire
        functionality of the layers listed below, and supersedes them. As the other layers
        are deprecated this layer should be used for all validation going forward.

        VK_EXT_debug_utils
        ==================
        Create a debug messenger which will pass along debug messages to an application supplied callback.
        Identify specific Vulkan objects using a name or tag to improve tracking.
        Identify specific sections within a VkQueue or VkCommandBuffer using labels to aid organization and offline analysis in external tools.

        */
        std::vector<const char*> extensions_device = { "VK_KHR_swapchain", "VK_EXT_memory_budget", "VK_EXT_depth_clip_enable", "VK_KHR_timeline_semaphore" };
        std::vector<const char*> validation_layers = { "VK_LAYER_KHRONOS_validation" };
        std::vector<VkValidationFeatureEnableEXT> validation_extensions = { VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT, VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT };
        std::vector<const char*> extensions_instance = { "VK_KHR_surface", "VK_KHR_win32_surface", "VK_EXT_debug_report", "VK_EXT_debug_utils" };
#else
        std::vector<const char*> extensions_device = { "VK_KHR_swapchain", "VK_EXT_memory_budget", "VK_EXT_depth_clip_enable", "VK_KHR_timeline_semaphore" };
        std::vector<const char*> validation_layers = { };
        std::vector<VkValidationFeatureEnableEXT> validation_extensions = { };
        std::vector<const char*> extensions_instance = { "VK_KHR_surface", "VK_KHR_win32_surface" };
#endif

        bool initalise_allocator();
        void destroy_allocator();

		// Queues
        VkQueue queue_graphics{};
        VkQueue queue_compute{};
        VkQueue queue_transfer{};
		uint32_t queue_graphics_index = 0;
		uint32_t queue_transfer_index = 0;
		uint32_t queue_compute_index = 0;
	};
}
