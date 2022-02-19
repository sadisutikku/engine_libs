


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/descriptor_set_layout_vulkan.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/descriptor_set_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
#include "gui/shared/graphics/RHI/descriptor.h"

namespace
{
	VkDescriptorType ToVulkanDescriptorType( const gui::DESCRIPTOR& descriptor )
	{
		if ( descriptor.type == gui::RHI_Descriptor_Type::ConstantBuffer )
			return descriptor.is_dynamic_constant_buffer ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		if ( descriptor.type == gui::RHI_Descriptor_Type::Texture )
			return descriptor.is_storage ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE : VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

		if ( descriptor.type == gui::RHI_Descriptor_Type::Sampler )
			return VK_DESCRIPTOR_TYPE_SAMPLER;

		ASSERT_FAILED( "Invalid descriptor type" );
		return VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}
}

namespace gui
{
	DESCRIPTOR_SET_LAYOUT_VULKAN::DESCRIPTOR_SET_LAYOUT_VULKAN( RENDERING_DEVICE_VULKAN& device, const std::vector<DESCRIPTOR>& descriptors )
		: m_device{ device }
		, m_descriptors{ descriptors }
	{
		CreateResource( m_descriptors );
		m_dynamic_offsets.fill( rhi_dynamic_offset_empty );
	}

	DESCRIPTOR_SET_LAYOUT_VULKAN::~DESCRIPTOR_SET_LAYOUT_VULKAN()
	{

	}

	void DESCRIPTOR_SET_LAYOUT_VULKAN::CreateResource( const std::vector<DESCRIPTOR>& descriptors )
	{
		ASSERT_TRUE( m_resource );

		// Layout bindings
		constexpr uint8_t descriptors_max = 255;
		std::array<VkDescriptorSetLayoutBinding, descriptors_max> layout_bindings;

		for ( uint32_t i = 0; i < static_cast<uint32_t>(descriptors.size()); i++ )
		{
			const DESCRIPTOR& descriptor = descriptors[i];

			// Stage flags
			VkShaderStageFlags stage_flags = 0;
			stage_flags |= (descriptor.stage & (uint32_t)RHI_Shader_Type::RHI_Shader_Vertex) ? VK_SHADER_STAGE_VERTEX_BIT : 0;
			stage_flags |= (descriptor.stage & (uint32_t)RHI_Shader_Type::RHI_Shader_Pixel) ? VK_SHADER_STAGE_FRAGMENT_BIT : 0;
			stage_flags |= (descriptor.stage & (uint32_t)RHI_Shader_Type::RHI_Shader_Compute) ? VK_SHADER_STAGE_COMPUTE_BIT : 0;

			layout_bindings[i].binding = descriptor.slot;
			layout_bindings[i].descriptorType = ToVulkanDescriptorType( descriptor );
			layout_bindings[i].descriptorCount = 1;
			layout_bindings[i].stageFlags = stage_flags;
			layout_bindings[i].pImmutableSamplers = nullptr;
		}

		// Create info
		VkDescriptorSetLayoutCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		create_info.flags = 0;
		create_info.pNext = nullptr;
		create_info.bindingCount = static_cast<uint32_t>(descriptors.size());
		create_info.pBindings = layout_bindings.data();

		// Descriptor set layout
		if ( vkCreateDescriptorSetLayout( m_device.get_context()->device, &create_info, nullptr, reinterpret_cast<VkDescriptorSetLayout*>(&m_resource) ) != VK_SUCCESS )
			return;

		//vulkan_utility::debug::set_name( m_resource, m_name.c_str() );
	}
}
