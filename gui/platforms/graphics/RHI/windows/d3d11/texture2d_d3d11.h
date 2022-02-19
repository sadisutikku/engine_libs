
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/texture2d_base.h"

namespace gui
{
	class RENDERER_D3D11;

	class TEXTURE2D_D3D11 : public TEXTURE2D_BASE
	{
	public:
		TEXTURE2D_D3D11( RENDERER_D3D11& renderer, uint32_t width, uint32_t height, RHI_Format format, uint16_t flags, const std::vector<std::byte>& data );
		~TEXTURE2D_D3D11() = default;

	private:
		bool create_texture2d_d3d11( RENDERER_D3D11& renderer );
	};
}
