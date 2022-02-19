
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/platforms/include_wrappers/windows/windows.h"

#include <atomic>
#include <gui/thirdparty/vulkan/vulkan.h>
#include <unordered_map>
#include <vector>

namespace gui
{
	class DESCRIPTOR_SET_LAYOUT_VULKAN;
	class PIPELINE_STATE_VULKAN;
	class RENDERING_DEVICE_VULKAN;
	struct DESCRIPTOR;

	class DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN
	{
	public:
		DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN( RENDERING_DEVICE_VULKAN& device );
		~DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN();

		DESCRIPTOR_SET_LAYOUT_VULKAN* GetCurrentDescriptorSetLayout() const { return m_descriptor_layout_current; }
		VkDescriptorPool GetResource_DescriptorPool() const { return m_descriptor_pool; }

		void SetPipelineState( PIPELINE_STATE_VULKAN& pipeline_state );

		void GetDescriptors( PIPELINE_STATE_VULKAN& pipeline_state, std::vector<DESCRIPTOR>& descriptors );

	private:
		void Reset( uint32_t descriptor_set_capacity );

		void SetDescriptorSetCapacity( uint32_t descriptor_set_capacity );

		bool CreateDescriptorPool( uint32_t descriptor_set_capacity );

		std::atomic<bool> m_descriptor_set_layouts_being_cleared{};
		uint32_t m_descriptor_set_capacity = 16;
		RENDERING_DEVICE_VULKAN& m_device;
		std::vector<DESCRIPTOR> m_descriptors;
		VkDescriptorPool m_descriptor_pool{};
		DESCRIPTOR_SET_LAYOUT_VULKAN* m_descriptor_layout_current{};
		std::unordered_map<std::size_t, DESCRIPTOR_SET_LAYOUT_VULKAN*> m_descriptor_set_layouts;
	};
}
