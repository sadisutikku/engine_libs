


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/descriptor_set_vulkan.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/descriptor_set_layout_cache_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/descriptor_set_layout_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"

namespace gui
{
	DESCRIPTOR_SET_VULKAN::DESCRIPTOR_SET_VULKAN( RENDERING_DEVICE_VULKAN& device_vulkan )
	{
		// Descriptor set layouts
		std::array<VkDescriptorSetLayout, 1> descriptor_set_layouts = { m_descriptor_set_layout_cache->GetCurrentDescriptorSetLayout()->get_resource() };

		// Allocate info
		VkDescriptorSetAllocateInfo allocate_info = {};
		allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocate_info.descriptorPool = static_cast<VkDescriptorPool>(m_descriptor_set_layout_cache->GetResource_DescriptorPool());
		allocate_info.descriptorSetCount = 1;
		allocate_info.pSetLayouts = descriptor_set_layouts.data();

		// Allocate
		[[maybe_unused]] const auto result = vkAllocateDescriptorSets( device_vulkan.get_context()->device, &allocate_info, &m_descriptor_set ) != VK_SUCCESS;
		ASSERT_TRUE( result );

		// Name
		//vulkan_utility::debug::set_name( *reinterpret_cast<VkDescriptorSet*>(&m_resource), m_name.c_str() );
	}

	DESCRIPTOR_SET_VULKAN::~DESCRIPTOR_SET_VULKAN()
	{

	}
}
