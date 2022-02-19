
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <memory>

namespace gui
{
	class RENDERING_DEVICE_BASE;
	enum class RHI_Api_Type : uint8_t;

	class RENDERING_DEVICE
	{
	public:
		RENDERING_DEVICE( RHI_Api_Type api_type );
		~RENDERING_DEVICE();

	private:
		std::unique_ptr<RENDERING_DEVICE_BASE> m_device;
	};
}
