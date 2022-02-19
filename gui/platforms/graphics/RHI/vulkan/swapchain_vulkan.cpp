


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/swapchain_vulkan.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/utilities_vulkan.h"
#include "gui/shared/graphics/RHI/RHI_types.h"

#include <algorithm>

#if PLATFORM_WINDOWS
#include <gui/thirdparty/vulkan/vulkan_win32.h>
#endif

namespace
{
	inline VkSurfaceCapabilitiesKHR get_capabilities( gui::RENDERING_DEVICE_VULKAN& device_vulkan, const VkSurfaceKHR surface )
	{
		VkSurfaceCapabilitiesKHR surface_capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device_vulkan.get_context()->device_physical, surface, &surface_capabilities );
		return surface_capabilities;
	}

	inline std::vector<VkPresentModeKHR> get_present_modes( gui::RENDERING_DEVICE_VULKAN& device_vulkan, const VkSurfaceKHR surface )
	{
		uint32_t present_mode_count;
		vkGetPhysicalDeviceSurfacePresentModesKHR( device_vulkan.get_context()->device_physical, surface, &present_mode_count, nullptr );

		std::vector<VkPresentModeKHR> surface_present_modes( present_mode_count );
		vkGetPhysicalDeviceSurfacePresentModesKHR( device_vulkan.get_context()->device_physical, surface, &present_mode_count, &surface_present_modes[0] );
		return surface_present_modes;
	}

	inline std::vector<VkSurfaceFormatKHR> formats( gui::RENDERING_DEVICE_VULKAN& device_vulkan, const VkSurfaceKHR surface )
	{
		uint32_t format_count;
		vkGetPhysicalDeviceSurfaceFormatsKHR( device_vulkan.get_context()->device_physical, surface, &format_count, nullptr );

		std::vector<VkSurfaceFormatKHR> surface_formats( format_count );
		vkGetPhysicalDeviceSurfaceFormatsKHR( device_vulkan.get_context()->device_physical, surface, &format_count, &surface_formats[0] );

		return surface_formats;
	}

	inline void detect_format_and_color_space( gui::RENDERING_DEVICE_VULKAN& device_vulkan, const VkSurfaceKHR surface, VkFormat* format, VkColorSpaceKHR* color_space )
	{
		std::vector<VkSurfaceFormatKHR> surface_formats = formats( device_vulkan, surface );

		// If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
		// there is no preferred format, so we assume VK_FORMAT_B8G8R8A8_UNORM
		if ( (surface_formats.size() == 1) && (surface_formats[0].format == VK_FORMAT_UNDEFINED) )
		{
			*format = VK_FORMAT_B8G8R8A8_UNORM;
			*color_space = surface_formats[0].colorSpace;
		}
		else
		{
			// iterate over the list of available surface format and
			// check for the presence of VK_FORMAT_B8G8R8A8_UNORM
			bool found_B8G8R8A8_UNORM = false;
			for ( auto&& surfaceFormat : surface_formats )
			{
				if ( surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM )
				{
					*format = surfaceFormat.format;
					*color_space = surfaceFormat.colorSpace;
					found_B8G8R8A8_UNORM = true;
					break;
				}
			}

			// in case VK_FORMAT_B8G8R8A8_UNORM is not available
			// select the first available color format
			if ( !found_B8G8R8A8_UNORM )
			{
				*format = surface_formats[0].format;
				*color_space = surface_formats[0].colorSpace;
			}
		}
	}

	inline VkPresentModeKHR set_present_mode( gui::RENDERING_DEVICE_VULKAN& device_vulkan, const VkSurfaceKHR surface, const uint32_t flags )
	{
		// Get preferred present mode
		VkPresentModeKHR present_mode_preferred = VK_PRESENT_MODE_FIFO_KHR;
		present_mode_preferred = flags & gui::RHI_Present_Immediate ? VK_PRESENT_MODE_IMMEDIATE_KHR : present_mode_preferred;
		present_mode_preferred = flags & gui::RHI_Present_Fifo ? VK_PRESENT_MODE_MAILBOX_KHR : present_mode_preferred;
		present_mode_preferred = flags & gui::RHI_Present_FifoRelaxed ? VK_PRESENT_MODE_FIFO_RELAXED_KHR : present_mode_preferred;
		present_mode_preferred = flags & gui::RHI_Present_SharedDemandRefresh ? VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR : present_mode_preferred;
		present_mode_preferred = flags & gui::RHI_Present_SharedDContinuousRefresh ? VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR : present_mode_preferred;

		// Check if the preferred mode is supported
		VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR; // VK_PRESENT_MODE_FIFO_KHR is always present (as per spec)
		std::vector<VkPresentModeKHR> surface_present_modes = get_present_modes( device_vulkan, surface );
		for ( const auto& supported_present_mode : surface_present_modes )
		{
			if ( present_mode_preferred == supported_present_mode )
			{
				present_mode = present_mode_preferred;
				break;
			}
		}

		return present_mode;
	}
}

namespace gui
{
	SWAPCHAIN_VULKAN::SWAPCHAIN_VULKAN( HWND window_handle, RENDERING_DEVICE_VULKAN& device_vulkan, const float width, const float height,
										RHI_Format format, uint32_t flags, const char* name )
		: m_device{ device_vulkan }
		, m_window_handle{ window_handle }
	{
		m_dimensions = { width, height };

		// Validate window handle
		if ( !window_handle || !IsWindow( window_handle ) )
		{
			ASSERT_FAILED( "Invalid window." );
			return;
		}

		//if ( !m_device.ValidateResolution( static_cast<uint32_t>(width), static_cast<uint32_t>(height) ) )
		//{
		//	//LOG_WARNING( "%dx%d is an invalid resolution", width, height );
		//	return;
		//}

		m_format = format;
		m_flags = flags;

		create_swapchain();

		// Create command pool
		command_pool::create( m_device, m_cmd_pool, RHI_Queue_Graphics );

		// Create command lists
		/*for ( uint32_t i = 0; i < m_buffer_count; i++ )
		{
			m_cmd_lists.emplace_back( make_shared<RHI_CommandList>( i, this, rhi_device->GetContext() ) );
		}*/

		AcquireNextImage();
	}

	SWAPCHAIN_VULKAN::~SWAPCHAIN_VULKAN()
	{
		// Wait in case any command buffer is still in use
		//m_device.Queue_WaitAll();

		// Command buffers
		//m_cmd_lists.clear();

		// Command pool
		command_pool::destroy( m_device, m_cmd_pool );

		// Resources
		destroy_swapchain();
	}

	bool SWAPCHAIN_VULKAN::create_swapchain()
	{
		// Create surface
		VkSurfaceKHR surface = nullptr;
		{
			VkWin32SurfaceCreateInfoKHR create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			create_info.hwnd = m_window_handle;
			create_info.hinstance = GetModuleHandle( nullptr );

			if ( vkCreateWin32SurfaceKHR( m_device.get_context()->instance, &create_info, nullptr, &surface ) != VK_SUCCESS )
				return false;

			VkBool32 present_support = false;
			if ( vkGetPhysicalDeviceSurfaceSupportKHR( m_device.get_context()->device_physical, m_device.get_context()->queue_graphics_index, surface, &present_support ) != VK_SUCCESS )
				return false;

			if ( !present_support )
			{
				ASSERT_FAILED( "The device does not support this kind of surface." );
				return false;
			}

			// Get surface capabilities
			VkSurfaceCapabilitiesKHR capabilities = get_capabilities( m_device, surface );

			// Compute extent
			m_dimensions.x = std::clamp( m_dimensions.x, (float)capabilities.minImageExtent.width, (float)capabilities.maxImageExtent.width );
			m_dimensions.y = std::clamp( m_dimensions.y, (float)capabilities.minImageExtent.height, (float)capabilities.maxImageExtent.height );
			VkExtent2D extent = { (uint32_t)m_dimensions.x, (uint32_t)m_dimensions.y };

			// Detect surface format and color space
			detect_format_and_color_space( m_device, surface, &m_device.get_context()->surface_format, &m_device.get_context()->surface_color_space );

			// Swap chain
			VkSwapchainKHR swap_chain;
			{
				VkSwapchainCreateInfoKHR create_info = {};
				create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				create_info.surface = surface;
				create_info.minImageCount = s_swap_chain_buffer_count;
				create_info.imageFormat = m_device.get_context()->surface_format;
				create_info.imageColorSpace = m_device.get_context()->surface_color_space;
				create_info.imageExtent = extent;
				create_info.imageArrayLayers = 1;
				create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

				uint32_t queueFamilyIndices[] = { m_device.get_context()->queue_compute_index, m_device.get_context()->queue_graphics_index };
				if ( m_device.get_context()->queue_compute_index != m_device.get_context()->queue_graphics_index )
				{
					create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
					create_info.queueFamilyIndexCount = 2;
					create_info.pQueueFamilyIndices = queueFamilyIndices;
				}
				else
				{
					create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
					create_info.queueFamilyIndexCount = 0;
					create_info.pQueueFamilyIndices = nullptr;
				}

				create_info.preTransform = capabilities.currentTransform;
				create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				create_info.presentMode = set_present_mode( m_device, surface, m_flags );
				create_info.clipped = VK_TRUE;
				create_info.oldSwapchain = nullptr;

				if ( vkCreateSwapchainKHR( m_device.get_context()->device, &create_info, nullptr, &swap_chain ) != VK_SUCCESS )
					return false;
			}

			// Images
			uint32_t image_count;
			std::vector<VkImage> images;
			{
				// Get
				vkGetSwapchainImagesKHR( m_device.get_context()->device, swap_chain, &image_count, nullptr );
				images.resize( image_count );
				vkGetSwapchainImagesKHR( m_device.get_context()->device, swap_chain, &image_count, images.data() );

				// Transition layouts to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				if ( VkCommandBuffer cmd_buffer = command_buffer_immediate::begin( m_device, RHI_Queue_Graphics ) )
				{
					for ( VkImage& image : images )
					{
						image::set_layout( m_device, reinterpret_cast<void*>(cmd_buffer), reinterpret_cast<void*>(image), VK_IMAGE_ASPECT_COLOR_BIT, 1, 1, RHI_Image_Layout::Undefined, RHI_Image_Layout::Present_Src );
					}

					// End/flush
					if ( !command_buffer_immediate::end( RHI_Queue_Graphics ) )
						return false;
				}
			}

			// Image views
			{
				for ( uint32_t i = 0; i < image_count; i++ )
				{
					m_images[i] = images[i];

					// Name the image
					//vulkan_utility::debug::set_name( images[i], std::string( std::string( "swapchain_image_" ) + std::to_string( i ) ).c_str() );

					if ( !image::view::create( m_device, static_cast<void*>(images[i]), m_resource_views[i], VK_IMAGE_VIEW_TYPE_2D, m_device.get_context()->surface_format, VK_IMAGE_ASPECT_COLOR_BIT ) )
						return false;
				}
			}

			m_surface = surface;
			m_swap_chain = swap_chain;

			// Semaphores
			/*for ( uint32_t i = 0; i < s_swap_chain_buffer_count; i++ )
			{
				image_acquired_semaphore[i] = std::make_shared<RHI_Semaphore>( rhi_device, false, (std::string( "swapchain_image_acquired_semaphore_" ) + to_string( i )).c_str() );
			}*/

			return true;
		}
	}

	void SWAPCHAIN_VULKAN::destroy_swapchain()
	{
		// Image views
		image::view::destroy( m_device, m_resource_views );

		// Swap chain view
		if ( m_swap_chain )
		{
			vkDestroySwapchainKHR( m_device.get_context()->device, m_swap_chain, nullptr );
			m_swap_chain = nullptr;
		}

		// Surface
		if ( m_surface )
		{
			vkDestroySurfaceKHR( m_device.get_context()->instance, m_surface, nullptr );
			m_surface = nullptr;
		}
	}

	bool SWAPCHAIN_VULKAN::AcquireNextImage()
	{
		if ( !m_present_enabled )
			return true;

		// Get next cmd index
		//uint32_t next_cmd_index = (m_cmd_index + 1) % m_buffer_count;

		// Get signal semaphore
		//RHI_Semaphore* signal_semaphore = m_image_acquired_semaphore[next_cmd_index].get();

		// Validate semaphore state
		//SP_ASSERT( signal_semaphore->GetState() == RHI_Semaphore_State::Idle );

		// Acquire next image
		VkResult result = vkAcquireNextImageKHR(
			m_device.get_context()->device,                      // device
			m_swap_chain,             // swapchain
			std::numeric_limits<uint64_t>::max(),                            // timeout
			nullptr,//static_cast<VkSemaphore>(signal_semaphore->GetResource()),  // signal semaphore
			nullptr,                                                    // signal fence
			&m_image_index                                              // pImageIndex
		);

		// Recreate swapchain with different size (if needed)
		if ( (result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR) )
		{
			//LOG_INFO( "Outdated swapchain, recreating..." );

			//if ( !Resize( m_width, m_height, true ) )
			{
				ASSERT_FAILED( "Failed to resize swapchain" );
				return false;
			}

			return AcquireNextImage();
		}

		// Check result
		if ( result != VK_SUCCESS)
		{
			ASSERT_FAILED( "Failed to acquire next image" );
			return false;
		}

		// Save cmd index
		//m_cmd_index = next_cmd_index;

		// Update semaphore state
		//signal_semaphore->SetState( RHI_Semaphore_State::Signaled );

		return true;
	}

	void SWAPCHAIN_VULKAN::present( /*RHI_Semaphore* wait_semaphore*/ )
	{
		// Validate swapchain state
		//SP_ASSERT( m_present_enabled );

		// Validate semaphore state
		//SP_ASSERT( wait_semaphore->GetState() == RHI_Semaphore_State::Signaled );

		// Acquire next image
		if ( !AcquireNextImage() )
		{
			ASSERT_FAILED( "Failed to acquire next image" );
			return;
		}

		// Present
		//if ( !m_device.Queue_Present( m_swap_chain, &m_image_index, nullptr/*wait_semaphore*/ ) )
		{
			ASSERT_FAILED( "Failed to present" );
			return;
		}
	}
}
