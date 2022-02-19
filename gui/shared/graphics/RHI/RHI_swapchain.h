
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <memory>

namespace gui
{
	class SWAPCHAIN_BASE;
	enum class RHI_Api_Type : uint8_t;

	class SWAPCHAIN
	{
	public:
		SWAPCHAIN( RHI_Api_Type api_type );
		~SWAPCHAIN();

	private:
		std::unique_ptr<SWAPCHAIN_BASE> m_swapchain;
	};
}
