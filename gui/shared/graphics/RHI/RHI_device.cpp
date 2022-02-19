
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_device.h"

#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
#include "gui/shared/graphics/RHI/RHI_types.h"

#if PLATFORM_WINDOWS
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"
#endif


namespace gui
{
	RENDERING_DEVICE::RENDERING_DEVICE( const RHI_Api_Type api_type )
	{
		switch ( api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
#if PLATFORM_WINDOWS
				m_device = std::make_unique<RENDERING_DEVICE_D3D11>();
#endif
				break;
			}
		case RHI_Api_Type::RHI_Api_Vulkan:
			{
				m_device = std::make_unique<RENDERING_DEVICE_VULKAN>();
				break;
			}
		}
	}

	RENDERING_DEVICE::~RENDERING_DEVICE() = default;
}
