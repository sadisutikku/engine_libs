
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/shader_base.h"

namespace gui
{
	class SHADER_D3D11 : public SHADER_BASE
	{
	public:
		SHADER_D3D11();
		~SHADER_D3D11();

	private:
		void* api_compile( RENDERING_DEVICE_BASE& device, const std::string& shader ) override;
	};
}
