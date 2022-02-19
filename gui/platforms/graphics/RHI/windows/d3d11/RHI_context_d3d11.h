
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_context_base.h"

#include "core/platforms/include_wrappers/windows/windows.h"
#include "core/shared/math/rectangle.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/d3d11_types.h"

#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma warning(push, 0) // Hide warnings which belong DirectX
#include <d3d11_4.h>
#pragma warning(pop)

//////////////////////////////////////////////////////////////////////
//! Forward Decelerations
//////////////////////////////////////////////////////////////////////

namespace core
{
	template<typename TYPE>
	struct VECTOR3D;
}

namespace gui
{
	class BUFFER_D3D11;
	class SAMPLER_D3D11;
	class TEXTURE2D_D3D11;
	class VIEWPORT;
	struct PIPELINE_STATE_D3D11;
	enum class SAMPLER_SLOTS : uint8_t;
	enum class TEXTURE_SLOTS : uint8_t;

	class RENDERING_CONTEXT_D3D11 : public RENDERING_CONTEXT_BASE
	{
	public:
		bool begin_render_pass( const PIPELINE_STATE_D3D11& pipeline_state );

		void clear_render_target_view( ID3D11RenderTargetView* render_target_view, const core::VECTOR3D<float>& rgb );

		void set_viewport( const VIEWPORT& viewport ) const;

		void set_vertex_buffer( BUFFER_D3D11& buffer_d3d11, uint32_t offset );

		void set_index_buffer( BUFFER_D3D11& buffer_d3d11, uint32_t offset );

		void set_constant_buffer( uint32_t slot, uint8_t shader_types, const BUFFER_D3D11& buffer_d3d11 );

		void set_texture( TEXTURE_SLOTS texture_slot, TEXTURE2D_D3D11* texture );

		void set_sampler( SAMPLER_SLOTS sampler_slot, SAMPLER_D3D11* sampler );

		void set_scissor_rect( const core::FRECTANGLE& rect );

		bool draw( const uint32_t vertex_count, const uint32_t vertex_offset );

		bool draw_indexed( uint32_t index_count, uint32_t index_offset, uint32_t vertex_offset );

		RHI_Api_Type m_api_type{ RHI_Api_Type::RHI_Api_D3d11 };
		Microsoft::WRL::ComPtr<ID3D11DeviceContext4> m_device_context;
		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> m_annotation;
	};
}
