
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_types.h"

#include <array>
#include <gui/thirdparty/vulkan/vulkan.h>
#include <vector>

namespace gui
{
	class RENDERING_DEVICE_VULKAN;
	struct DESCRIPTOR;

	class DESCRIPTOR_SET_LAYOUT_VULKAN
	{
	public:
		DESCRIPTOR_SET_LAYOUT_VULKAN( RENDERING_DEVICE_VULKAN& device, const std::vector<DESCRIPTOR>& descriptors );
		~DESCRIPTOR_SET_LAYOUT_VULKAN();

		VkDescriptorSetLayout get_resource() const { return m_resource; }

	private:
		void CreateResource( const std::vector<DESCRIPTOR>& descriptors );

		RENDERING_DEVICE_VULKAN& m_device;
		VkDescriptorSetLayout m_resource{};
		std::vector<DESCRIPTOR> m_descriptors;
		std::array<uint32_t, rhi_max_constant_buffer_count> m_dynamic_offsets;
	};
}
