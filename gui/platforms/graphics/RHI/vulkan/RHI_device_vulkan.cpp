


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"

#include "core/shared/debugging/asserts.h"
#include "gui/shared/graphics/RHI/RHI_pyhsical_device.h"
#include "gui/shared/graphics/RHI/RHI_types.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"

#include <algorithm>
#include <string>

namespace
{
	inline bool is_present_instance( const char* extension_name )
	{
		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, nullptr );

		std::vector<VkExtensionProperties> extensions( extension_count );
		vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, extensions.data() );

		for ( const auto& extension : extensions )
		{
			if (strcmp(extension_name, extension.extensionName) == 0)
			{
				return true;
			}
		}

		return false;
	}

	inline std::vector<const char*> get_supported_instance( const std::vector<const char*>& extensions )
	{
		std::vector<const char*> extensions_supported;

		for ( const auto& extension : extensions )
		{
			if ( is_present_instance( extension ) )
			{
				extensions_supported.emplace_back( extension );
			}
			else
			{
				//ASSERT_FAILED( "Instance extension \"%s\" is not supported", extension );
			}
		}

		return extensions_supported;
	}

	inline bool is_present( const char* layer_name )
	{
		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties( &layer_count, nullptr );

		std::vector<VkLayerProperties> layers( layer_count );
		vkEnumerateInstanceLayerProperties( &layer_count, layers.data() );

		for ( const auto& layer : layers )
		{
			if ( strcmp( layer_name, layer.layerName ) == 0 )
				return true;
		}

		return false;
	}

	PFN_vkCreateDebugUtilsMessengerEXT           create_messenger;
	VkDebugUtilsMessengerEXT                     messenger;
	PFN_vkDestroyDebugUtilsMessengerEXT          destroy_messenger;
	PFN_vkSetDebugUtilsObjectTagEXT              set_object_tag;
	PFN_vkSetDebugUtilsObjectNameEXT             set_object_name;
	PFN_vkCmdBeginDebugUtilsLabelEXT             marker_begin;
	PFN_vkCmdEndDebugUtilsLabelEXT               marker_end;
	PFN_vkGetPhysicalDeviceMemoryProperties2KHR  get_physical_device_memory_properties_2;

	template<typename TYPE>
	TYPE get_vulkan_function( gui::RENDERING_CONTEXT_VULKAN* context, const char* function_name )
	{
		auto function_ptr = reinterpret_cast<TYPE>(vkGetInstanceProcAddr( static_cast<VkInstance>(context->instance), function_name ));
		ASSERT_TRUE( function_ptr );
		return function_ptr;
	}

	void initialise_vulkan_functions( gui::RENDERING_CONTEXT_VULKAN* context )
	{
		get_physical_device_memory_properties_2 = get_vulkan_function<PFN_vkGetPhysicalDeviceMemoryProperties2KHR>( context, "vkGetPhysicalDeviceMemoryProperties2" );

		if ( context->debug )
		{
			/* VK_EXT_debug_utils */
			create_messenger = get_vulkan_function<PFN_vkCreateDebugUtilsMessengerEXT>( context, "vkCreateDebugUtilsMessengerEXT" );
			destroy_messenger = get_vulkan_function<PFN_vkDestroyDebugUtilsMessengerEXT>( context, "vkDestroyDebugUtilsMessengerEXT" );
			marker_begin = get_vulkan_function<PFN_vkCmdBeginDebugUtilsLabelEXT>( context, "vkCmdBeginDebugUtilsLabelEXT" );
			marker_end = get_vulkan_function<PFN_vkCmdEndDebugUtilsLabelEXT>( context, "vkCmdEndDebugUtilsLabelEXT" );

			/* VK_EXT_debug_marker */
			set_object_tag = get_vulkan_function<PFN_vkSetDebugUtilsObjectTagEXT>( context, "vkSetDebugUtilsObjectTagEXT" );
			set_object_name = get_vulkan_function<PFN_vkSetDebugUtilsObjectNameEXT>( context, "vkSetDebugUtilsObjectNameEXT" );
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL callback( VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity, VkDebugUtilsMessageTypeFlagsEXT msg_type, const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, void* p_user_data )
	{
		std::string msg = "Vulkan: " + std::string( p_callback_data->pMessage );

		if ( msg_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT )
		{
			//Log::Write( msg.c_str(), LogType::Info );
		}
		else if ( msg_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT )
		{
			//Log::Write( msg.c_str(), LogType::Info );
		}
		else if ( msg_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT )
		{
			//Log::Write( msg.c_str(), LogType::Warning );
		}
		else if ( msg_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT )
		{
			//Log::Write( msg.c_str(), LogType::Error );
			ASSERT_SHOULDNT_BE_HERE();
		}

		return VK_FALSE;
	}

	void initialise_messenger( VkInstance instance )
	{
		if ( create_messenger )
		{
			VkDebugUtilsMessengerCreateInfoEXT create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			create_info.pfnUserCallback = callback;

			if ( create_messenger( instance, &create_info, nullptr, &messenger ) != VK_SUCCESS )
			{
				ASSERT_FAILED( "" );
			}
		}
	}

	inline uint32_t get_queue_family_index( VkQueueFlagBits queue_flags, const std::vector<VkQueueFamilyProperties>& queue_family_properties, uint32_t* index )
	{
		// Dedicated queue for compute
		// Try to find a queue family index that supports compute but not graphics
		if ( queue_flags & VK_QUEUE_COMPUTE_BIT )
		{
			for ( uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); i++ )
			{
				if ( (queue_family_properties[i].queueFlags & queue_flags) && ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) )
				{
					*index = i;
					return true;
				}
			}
		}

		// Dedicated queue for transfer
		// Try to find a queue family index that supports transfer but not graphics and compute
		if ( queue_flags & VK_QUEUE_TRANSFER_BIT )
		{
			for ( uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); i++ )
			{
				if ( (queue_family_properties[i].queueFlags & queue_flags) && ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0) )
				{
					*index = i;
					return true;
				}
			}
		}

		// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
		for ( uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); i++ )
		{
			if ( queue_family_properties[i].queueFlags & queue_flags )
			{
				*index = i;
				return true;
			}
		}

		return false;
	}

	inline bool get_queue_family_indices( gui::RENDERING_CONTEXT_VULKAN* context, const VkPhysicalDevice& physical_device )
	{
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties( physical_device, &queue_family_count, nullptr );

		std::vector<VkQueueFamilyProperties> queue_families_properties( queue_family_count );
		vkGetPhysicalDeviceQueueFamilyProperties( physical_device, &queue_family_count, queue_families_properties.data() );

		if ( !get_queue_family_index( VK_QUEUE_GRAPHICS_BIT, queue_families_properties, &context->queue_graphics_index ) )
		{
			ASSERT_FAILED( "Graphics queue not suported." );
			return false;
		}

		if ( !get_queue_family_index( VK_QUEUE_TRANSFER_BIT, queue_families_properties, &context->queue_transfer_index ) )
		{
			//LOG_WARNING( "Transfer queue not suported, using graphics instead." );
			context->queue_transfer_index = context->queue_graphics_index;
		}

		if ( !get_queue_family_index( VK_QUEUE_COMPUTE_BIT, queue_families_properties, &context->queue_compute_index ) )
		{
			//LOG_WARNING( "Compute queue not suported, using graphics instead." );
			context->queue_compute_index = context->queue_graphics_index;
		}

		return true;
	}

	inline bool choose_physical_device( gui::RENDERING_DEVICE_VULKAN* device )
	{
		// Register all physical devices
		{
			uint32_t device_count = 0;
			if ( vkEnumeratePhysicalDevices( device->get_context()->instance, &device_count, nullptr ) != VK_SUCCESS )
				return false;

			if ( device_count == 0 )
			{
				ASSERT_FAILED( "There are no available devices." );
				return false;
			}

			std::vector<VkPhysicalDevice> physical_devices( device_count );
			if ( vkEnumeratePhysicalDevices( device->get_context()->instance, &device_count, physical_devices.data() ) != VK_SUCCESS )
				return false;

			// Go through all the devices
			for ( const VkPhysicalDevice& device_physical : physical_devices )
			{
				// Get device properties
				VkPhysicalDeviceProperties device_properties = {};
				vkGetPhysicalDeviceProperties( device_physical, &device_properties );

				VkPhysicalDeviceMemoryProperties device_memory_properties = {};
				vkGetPhysicalDeviceMemoryProperties( device_physical, &device_memory_properties );

				gui::RHI_PhysicalDevice_Type type = gui::RHI_PhysicalDevice_Type::RHI_PhysicalDevice_Unknown;
				if ( device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ) type = gui::RHI_PhysicalDevice_Type::RHI_PhysicalDevice_Integrated;
				else if ( device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )   type = gui::RHI_PhysicalDevice_Type::RHI_PhysicalDevice_Discrete;
				else if ( device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU )    type = gui::RHI_PhysicalDevice_Type::RHI_PhysicalDevice_Virtual;
				else if ( device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU )            type = gui::RHI_PhysicalDevice_Type::RHI_PhysicalDevice_Cpu;

				// Let the engine know about it as it will sort all of the devices from best to worst
				device->RegisterPhysicalDevice( gui::PYHSICAL_DEVICE(
															 device_properties.apiVersion,                                           // api version
															 device_properties.driverVersion,                                        // driver version
															 device_properties.vendorID,                                             // vendor id
															 type,                                                                   // type
															 &device_properties.deviceName[0],                                       // name
															 static_cast<uint32_t>(device_memory_properties.memoryHeaps[0].size),    // memory
															 static_cast<void*>(device_physical) ) );
			}
		}

		// Go through all the devices (sorted from best to worst based on their properties)
		for ( uint32_t device_index = 0; device_index < device->GetPhysicalDevices().size(); device_index++ )
		{
			const gui::PYHSICAL_DEVICE& physical_device = device->GetPhysicalDevices()[device_index];
			VkPhysicalDevice physical_device_vk = static_cast<VkPhysicalDevice>(physical_device.GetData());

			// Get the first device that has a graphics, a compute and a transfer queue
			if ( get_queue_family_indices( device->get_context(), physical_device_vk ) )
			{
				device->SetPrimaryPhysicalDevice( device_index );
				device->get_context()->device_physical = physical_device_vk;
				break;
			}
		}

		return true;
	}

	inline bool is_present_device( const char* extension_name, VkPhysicalDevice device_physical )
	{
		uint32_t extension_count = 0;
		vkEnumerateDeviceExtensionProperties( device_physical, nullptr, &extension_count, nullptr );

		std::vector<VkExtensionProperties> extensions( extension_count );
		vkEnumerateDeviceExtensionProperties( device_physical, nullptr, &extension_count, extensions.data() );

		for ( const auto& extension : extensions )
		{
			if ( strcmp( extension_name, extension.extensionName ) == 0 )
			{
				return true;
			}
		}

		return false;
	}

	inline std::vector<const char*> get_supported_device( const std::vector<const char*>& extensions, VkPhysicalDevice device_physical )
	{
		std::vector<const char*> extensions_supported;

		for ( const auto& extension : extensions )
		{
			if ( is_present_device( extension, device_physical ) )
			{
				extensions_supported.emplace_back( extension );
			}
			else
			{
				ASSERT_FAILED( "A device extension is not supported" );
			}
		}

		return extensions_supported;
	}

	void shutdown( VkInstance instance )
	{
		if ( !destroy_messenger )
		{
			return;
		}

		destroy_messenger( instance, messenger, nullptr );
	}
}

namespace gui
{
	RENDERING_DEVICE_VULKAN::RENDERING_DEVICE_VULKAN()
		: m_context{ std::make_unique<RENDERING_CONTEXT_VULKAN>() }
	{
		set_api_version();

		if ( !create_instance() )
		{
			ASSERT_FAILED( "Failed to create Vulkan instance" );
			return;
		}

		initialise_vulkan_functions( m_context.get() );

		// Debug
		if ( m_context->debug )
		{
			initialise_messenger( m_context->instance );
		}

		// Find a physical device
		if ( !choose_physical_device( this ) )
		{
			ASSERT_FAILED( "Failed to find a suitable physical device." );
			return;
		}


		if ( !create_device() )
		{
			ASSERT_FAILED( "Failed to create a Vulkan device." );
			return;
		}
		
		// Get device properties
		vkGetPhysicalDeviceProperties( m_context->device_physical, &m_context->device_properties );

		// Resource limits
		m_context->texture_2d_dimension_max = m_context->device_properties.limits.maxImageDimension2D;

		// Disable profiler if timestamps are not supported
		if ( m_context->profiler && !m_context->device_properties.limits.timestampComputeAndGraphics )
		{
			//LOG_WARNING( "Device doesn't support timestamps, disabling profiler..." );
			m_context->profiler = false;
		}

		// Get queues
		vkGetDeviceQueue( m_context->device, m_context->queue_graphics_index, 0, &m_context->queue_graphics );
		vkGetDeviceQueue( m_context->device, m_context->queue_transfer_index, 0, &m_context->queue_transfer );
		vkGetDeviceQueue( m_context->device, m_context->queue_compute_index, 0, &m_context->queue_compute );

		//detect_display_modes();

		// Initialise the memory allocator
		if ( !m_context->initalise_allocator() )
		{
			ASSERT_FAILED( "Failed to initialise Vulkan allocator" );
		}
	}

	RENDERING_DEVICE_VULKAN::~RENDERING_DEVICE_VULKAN()
	{
		if ( !m_context || !m_context->queue_graphics )
		{
			return;
		}

		// Release resources
		if ( Queue_WaitAll() )
		{
			m_context->destroy_allocator();

			if ( m_context->debug )
			{
				shutdown( m_context->instance );
			}

			vkDestroyDevice( m_context->device, nullptr );
			vkDestroyInstance( m_context->instance, nullptr );
		}
	}

	void RENDERING_DEVICE_VULKAN::set_api_version()
	{
		// Get sdk version
		uint32_t sdk_version{ VK_HEADER_VERSION_COMPLETE };

		// Get driver version
		uint32_t driver_version{ 0 };

		// Per to LunarG, if vkEnumerateInstanceVersion is not present, we are running on Vulkan 1.0
		// https://www.lunarg.com/wp-content/uploads/2019/02/Vulkan-1.1-Compatibility-Statement_01_19.pdf
		auto eiv = reinterpret_cast<PFN_vkEnumerateInstanceVersion>(vkGetInstanceProcAddr( nullptr, "vkEnumerateInstanceVersion" ));

		if ( eiv )
		{
			eiv( &driver_version );
		}
		else
		{
			driver_version = VK_API_VERSION_1_0;
		}

		// Choose the version which is supported by both the sdk and the driver
		m_context->api_version = std::min<uint32_t>( sdk_version, driver_version );

		// In case the SDK is not supported by the driver, prompt the user to update
		if ( sdk_version > driver_version )
		{
			// Detect and log version
			std::string driver_version_str = std::to_string( VK_VERSION_MAJOR( driver_version ) ) + "." + std::to_string( VK_VERSION_MINOR( driver_version ) ) + "." + std::to_string( VK_VERSION_PATCH( driver_version ) );
			std::string sdk_version_str = std::to_string( VK_VERSION_MAJOR( sdk_version ) ) + "." + std::to_string( VK_VERSION_MINOR( sdk_version ) ) + "." + std::to_string( VK_VERSION_PATCH( sdk_version ) );
			//ASSERT( "Falling back to Vulkan %s. Please update your graphics drivers to support Vulkan %s.", driver_version_str.c_str(), sdk_version_str.c_str() );
		}
	}

	bool RENDERING_DEVICE_VULKAN::create_instance()
	{
		// Create instance
		VkApplicationInfo app_info = {};

		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName = "application";
		app_info.pEngineName = "engine";
		app_info.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
		app_info.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
		app_info.apiVersion = m_context->api_version;

		// Get the supported extensions out of the requested extensions
		std::vector<const char*> extensions_supported = get_supported_instance( m_context->extensions_instance );

		VkInstanceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &app_info;
		create_info.enabledExtensionCount = static_cast<uint32_t>(extensions_supported.size());
		create_info.ppEnabledExtensionNames = extensions_supported.data();
		create_info.enabledLayerCount = 0;

		// Validation features
		VkValidationFeaturesEXT validation_features = {};
		validation_features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		validation_features.enabledValidationFeatureCount = static_cast<uint32_t>(m_context->validation_extensions.size());
		validation_features.pEnabledValidationFeatures = m_context->validation_extensions.data();

		if ( m_context->debug )
		{
			// Enable validation layer
			if ( is_present( m_context->validation_layers.front() ) )
			{
				// Validation layers
				create_info.enabledLayerCount = static_cast<uint32_t>(m_context->validation_layers.size());
				create_info.ppEnabledLayerNames = m_context->validation_layers.data();
				create_info.pNext = &validation_features;
			}
			else
			{
				//ASSERT_FAILED( "Validation layer was requested, but not available." );
			}
		}

		return vkCreateInstance( &create_info, nullptr, &m_context->instance ) == VK_SUCCESS;
	}

	bool RENDERING_DEVICE_VULKAN::create_device()
	{
		std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

		float queue_priority = 1.0f;
		for ( const uint32_t& queue_family : { m_context->queue_graphics_index, m_context->queue_transfer_index, m_context->queue_compute_index } )
		{
			VkDeviceQueueCreateInfo queue_create_info = {};
			queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.queueFamilyIndex = queue_family;
			queue_create_info.queueCount = 1;
			queue_create_info.pQueuePriorities = &queue_priority;
			queue_create_infos.push_back( queue_create_info );
		}

		// Get device features
		VkPhysicalDeviceVulkan12Features device_features_1_2_enabled = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
		VkPhysicalDeviceFeatures2 device_features_enabled = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, &device_features_1_2_enabled };

		auto enable_feature = []( const VkBool32 device_feature, VkBool32& enabled_feature )
		{
			if ( device_feature )
			{
				enabled_feature = VK_TRUE;
			}
			else
			{
				//LOG_WARNING( "Requested device feature " #feature " is not supported by the physical device" );
				enabled_feature = VK_FALSE;
			}
		};

		// Get
		vkGetPhysicalDeviceFeatures2( m_context->device_physical, &m_context->device_features );

		// Enable
		enable_feature( m_context->device_features.features.samplerAnisotropy, device_features_enabled.features.samplerAnisotropy );
		enable_feature( m_context->device_features.features.fillModeNonSolid, device_features_enabled.features.fillModeNonSolid );
		enable_feature( m_context->device_features.features.wideLines, device_features_enabled.features.wideLines );
		enable_feature( m_context->device_features.features.imageCubeArray, device_features_enabled.features.imageCubeArray );
		enable_feature( m_context->device_features_1_2.timelineSemaphore, device_features_1_2_enabled.timelineSemaphore );

		// Determine enabled graphics shader stages
		m_enabled_graphics_shader_stages = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		if ( device_features_enabled.features.geometryShader )
		{
			m_enabled_graphics_shader_stages |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
		}

		if ( device_features_enabled.features.tessellationShader )
		{
			m_enabled_graphics_shader_stages |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT | VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
		}

		// Get the supported extensions out of the requested extensions
		std::vector<const char*> extensions_supported = get_supported_device( m_context->extensions_device, m_context->device_physical );

		// Device create info
		VkDeviceCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
		create_info.pQueueCreateInfos = queue_create_infos.data();
		create_info.pNext = &device_features_enabled;
		create_info.enabledExtensionCount = static_cast<uint32_t>(extensions_supported.size());
		create_info.ppEnabledExtensionNames = extensions_supported.data();

		if ( m_context->debug )
		{
			create_info.enabledLayerCount = static_cast<uint32_t>(m_context->validation_layers.size());
			create_info.ppEnabledLayerNames = m_context->validation_layers.data();
		}
		else
		{
			create_info.enabledLayerCount = 0;
		}

		return vkCreateDevice( m_context->device_physical, &create_info, nullptr, &m_context->device ) == VK_SUCCESS;
	}

	void* RENDERING_DEVICE_VULKAN::Queue_Get( const RHI_Queue_Type type ) const
	{
		switch ( type )
		{
		case RHI_Queue_Graphics:
			{
				return m_context->queue_graphics;
			}
		case RHI_Queue_Transfer:
			{
				return m_context->queue_transfer;
			}
		case RHI_Queue_Compute:
			{
				return m_context->queue_compute;
			}
		}

		return nullptr;
	}

	uint32_t RENDERING_DEVICE_VULKAN::Queue_Index( const RHI_Queue_Type type ) const
	{
		switch ( type )
		{
		case RHI_Queue_Graphics:
			{
				return m_context->queue_graphics_index;
			}
		case RHI_Queue_Transfer:
			{
				return m_context->queue_transfer_index;
			}
		case RHI_Queue_Compute:
			{
				return m_context->queue_compute_index;
			}
		}

		return 0u;
	}

	bool RENDERING_DEVICE_VULKAN::Queue_Submit( const RHI_Queue_Type type, const uint32_t wait_flags, void* cmd_buffer, void* wait_semaphore /*= nullptr*/, void* signal_semaphore /*= nullptr*/, void* signal_fence /*= nullptr*/ ) const
	{
		// Validate input
		ASSERT_TRUE( cmd_buffer != nullptr );

		// Validate semaphore states
		//if ( wait_semaphore )     ASSERT_TRUE( wait_semaphore->GetState() == RHI_Semaphore_State::Signaled );
		//if ( signal_semaphore )   ASSERT_TRUE( signal_semaphore->GetState() == RHI_Semaphore_State::Idle );

		//// Get semaphore Vulkan resources
		//void* vk_wait_semaphore = wait_semaphore ? wait_semaphore->GetResource() : nullptr;
		//void* vk_signal_semaphore = signal_semaphore ? signal_semaphore->GetResource() : nullptr;

		// Submit info
		VkSubmitInfo submit_info = {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.pNext = nullptr;
		submit_info.waitSemaphoreCount = wait_semaphore ? 1 : 0;
		submit_info.pWaitSemaphores = nullptr;// wait_semaphore ? reinterpret_cast<VkSemaphore*>(&vk_wait_semaphore) : nullptr
		submit_info.signalSemaphoreCount = signal_semaphore ? 1 : 0;
		submit_info.pSignalSemaphores = nullptr;//signal_semaphore ? reinterpret_cast<VkSemaphore*>(&vk_signal_semaphore) : nullptr;
		submit_info.pWaitDstStageMask = &wait_flags;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = reinterpret_cast<VkCommandBuffer*>(&cmd_buffer);

		// Get signal fence
		void* vk_signal_fence = {};// signal_fence ? signal_fence->GetResource() : nullptr;

		std::lock_guard<std::mutex> lock( m_queue_mutex );
		if ( vkQueueSubmit( static_cast<VkQueue>(Queue_Get( type )), 1, &submit_info, static_cast<VkFence>(vk_signal_fence) ) != VK_SUCCESS )
			return false;

		// Update semaphore states
		/*if ( wait_semaphore )     wait_semaphore->SetState( RHI_Semaphore_State::Idle );
		if ( signal_semaphore )   signal_semaphore->SetState( RHI_Semaphore_State::Signaled );*/

		return true;
	}

	bool RENDERING_DEVICE_VULKAN::Queue_Wait( const RHI_Queue_Type type ) const
	{
		std::lock_guard<std::mutex> lock( m_queue_mutex );
		return vkQueueWaitIdle( static_cast<VkQueue>(Queue_Get( type )) ) == VK_SUCCESS;
	}

	bool RENDERING_DEVICE_VULKAN::Queue_WaitAll() const
	{
		return Queue_Wait( RHI_Queue_Graphics ) && Queue_Wait( RHI_Queue_Transfer ) && Queue_Wait( RHI_Queue_Compute );
	}
}
