
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/buffer_base.h"

#include "core/platforms/include_wrappers/windows/windows.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace gui
{
	class RENDERING_DEVICE_D3D11;

	class BUFFER_D3D11 : public BUFFER_BASE
	{
	public:
		void* Map( RENDERING_DEVICE_D3D11& device );

		bool Unmap( RENDERING_DEVICE_D3D11& device );

		ID3D11Buffer* GetResource() const { return m_buffer.Get(); }

	private:
		bool _create( RENDERING_DEVICE_BASE& device, BUFFER_TYPE buffer_type, const void* vertices ) override;

		void _destroy();

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	};
}
