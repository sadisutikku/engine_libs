

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_swapchain.h"

#include "gui/shared/screen/screen_manager.h"
#include "gui/shared/window/window.h"
#include "gui/shared/graphics/RHI/RHI_swapchain _base.h"
#include "gui/shared/graphics/RHI/RHI_types.h"

#if PLATFORM_WINDOWS
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_swapchain_d3d11.h"
#endif

namespace gui
{
	SWAPCHAIN::SWAPCHAIN( const RHI_Api_Type api_type )
	{
		const WINDOW* window = SCREEN_MANAGER::get_singleton()->get_window( SCREEN_MANAGER::MAIN_SCREEN_ID );
		const auto height = window->get_height();
		const auto width = window->get_width();
		void* window_handle = window->get_native_window();

		switch ( api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
#if PLATFORM_WINDOWS
				//m_swapchain = std::make_unique<SWAPCHAIN_D3D11>(  );
#endif
				break;
			}
		case RHI_Api_Type::RHI_Api_Vulkan:
			{
				
				break;
			}
		}
	}

	SWAPCHAIN::~SWAPCHAIN() = default;
}
