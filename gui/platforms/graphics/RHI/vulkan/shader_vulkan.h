
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/shader_base.h"

namespace gui
{
	class SHADER_VULKAN : public SHADER_BASE
	{
	public:
		SHADER_VULKAN();
		~SHADER_VULKAN();

	private:
		void* api_compile( RENDERING_DEVICE_BASE& device, const std::string& shader ) override;

		void _Reflect( RHI_Shader_Type shader_type, const uint32_t* ptr, uint32_t size );
	};
}
