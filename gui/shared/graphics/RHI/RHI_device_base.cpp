
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_device_base.h"

namespace gui
{
	RENDERING_DEVICE_BASE::RENDERING_DEVICE_BASE() = default;

	RENDERING_DEVICE_BASE::~RENDERING_DEVICE_BASE() = default;

	void RENDERING_DEVICE_BASE::RegisterPhysicalDevice( const PYHSICAL_DEVICE& physical_device )
	{
		m_physical_devices.emplace_back( physical_device );

		// Keep devices sorted, based on memory (from highest to lowest)
		/*sort( m_physical_devices.begin(), m_physical_devices.end(), []( const PhysicalDevice& adapter1, const PhysicalDevice& adapter2 )
			  {
				  return adapter1.GetMemory() > adapter2.GetMemory();
			  } );

		LOG_INFO( "%s (%d MB)", physical_device.GetName().c_str(), physical_device.GetMemory() );*/
	}
}
