
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/command_list.h"

#include "gui/shared/graphics/RHI/RHI_types.h"

#if PLATFORM_WINDOWS
#include "gui/platforms/graphics/RHI/windows/d3d11/command_list_d3d11.h"
#endif


namespace gui
{
	COMMAND_LIST::COMMAND_LIST( const RHI_Api_Type api_type )
	{
		switch ( api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
#if PLATFORM_WINDOWS
				m_command_list = std::make_unique<COMMAND_LIST_D3D11>();
#endif
				break;
			}
		case RHI_Api_Type::RHI_Api_Vulkan:
			{

				break;
			}
		}
	}

	COMMAND_LIST::~COMMAND_LIST() = default;
}
