
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/platforms/include_wrappers/windows/windows.h"

#include <gui/thirdparty/vulkan/vulkan.h>

namespace gui
{
	class DESCRIPTOR_SET_LAYOUT_VULKAN;
	class PIPELINE_STATE_VULKAN;
	class RENDERING_DEVICE_VULKAN;

	class PIPELINE_VULKAN
	{
	public:
		PIPELINE_VULKAN( RENDERING_DEVICE_VULKAN& device_vulkan, PIPELINE_STATE_VULKAN& pipeline_state, DESCRIPTOR_SET_LAYOUT_VULKAN* descriptor_set_layout );
		~PIPELINE_VULKAN();

	private:
		RENDERING_DEVICE_VULKAN& m_device;
		PIPELINE_STATE_VULKAN& m_pipeline_state;
		VkPipeline m_pipeline{};
		VkPipelineLayout m_pipeline_layout{};
	};
}
