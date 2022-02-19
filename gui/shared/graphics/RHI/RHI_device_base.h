
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_types.h"

#include "gui/shared/graphics/RHI/RHI_pyhsical_device.h"

#include <mutex>
#include <vector>

namespace gui
{
	struct RENDERING_CONTEXT_BASE;

	class RENDERING_DEVICE_BASE
	{
	public:
		RENDERING_DEVICE_BASE();
		~RENDERING_DEVICE_BASE();

		void RegisterPhysicalDevice( const PYHSICAL_DEVICE& physical_device );

		const std::vector<PYHSICAL_DEVICE>& GetPhysicalDevices() const { return m_physical_devices; }

		void SetPrimaryPhysicalDevice( const uint32_t index ) { m_physical_device_index = index; }

		const PYHSICAL_DEVICE* GetPrimaryPhysicalDevice() const { return &m_physical_devices[m_physical_device_index]; }

		uint32_t GetEnabledGraphicsStages() const { return m_enabled_graphics_shader_stages; }

		uint32_t m_enabled_graphics_shader_stages = 0;
		bool m_initialized = false;
		mutable std::mutex m_queue_mutex;

	private:
		std::vector<PYHSICAL_DEVICE> m_physical_devices;
		uint32_t m_physical_device_index{ 0 };
	};
}
