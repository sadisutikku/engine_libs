

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_context_d3d11.h"

#include "core/shared/math/vector3d.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/buffer_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/pipeline_state_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/sampler_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/shader_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/structured_buffer.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/rasteriser_state_d3d11.h"
#include "gui/shared/graphics/RHI/input_layout.h"
#include "gui/shared/graphics/RHI/viewport.h"

#include <d3d11.h>

namespace gui
{
	bool RENDERING_CONTEXT_D3D11::begin_render_pass( const PIPELINE_STATE_D3D11& pipeline_state )
	{
		// Vertex shader
		{
			// New state
			ID3D11VertexShader* shader = static_cast<ID3D11VertexShader*>(pipeline_state.m_vertex_shader ? pipeline_state.m_vertex_shader->GetResource() : nullptr);

			// Current state
			ID3D11VertexShader* set_shader = nullptr;
			UINT instance_count = 256;
			ID3D11ClassInstance* instances[256];
			m_device_context->VSGetShader( &set_shader, instances, &instance_count );

			// Set if dirty
			if ( set_shader != shader )
			{
				m_device_context->VSSetShader( shader, nullptr, 0u );
			}
		}

		// Vertex Shader Input layout
		{
			// New state
			ID3D11InputLayout* input_layout = static_cast<ID3D11InputLayout*>(pipeline_state.m_vertex_shader ? pipeline_state.m_vertex_shader->get_input_layout()->GetResource() : nullptr);

			// Current state
			ID3D11InputLayout* input_layout_set = nullptr;
			m_device_context->IAGetInputLayout( &input_layout_set );

			// Set if dirty
			if ( input_layout_set != input_layout )
			{
				m_device_context->IASetInputLayout( input_layout );
			}
		}

		// Pixel shader
		{
			// New state
			ID3D11PixelShader* shader = static_cast<ID3D11PixelShader*>(pipeline_state.m_pixel_shader ? pipeline_state.m_pixel_shader->GetResource() : nullptr);

			// Current state
			ID3D11PixelShader* set_shader = nullptr; UINT instance_count = 256; ID3D11ClassInstance* instances[256];
			m_device_context->PSGetShader( &set_shader, instances, &instance_count );

			// Set if dirty
			if ( set_shader != shader )
			{
				m_device_context->PSSetShader( shader, nullptr, 0 );
			}
		}

		// Compute shader
		{
			// New state
			ID3D11ComputeShader* shader = static_cast<ID3D11ComputeShader*>(pipeline_state.m_compute_shader ? pipeline_state.m_compute_shader->GetResource() : nullptr);

			// Current state
			ID3D11ComputeShader* set_shader = nullptr; UINT instance_count = 256; ID3D11ClassInstance* instances[256];
			m_device_context->CSGetShader( &set_shader, instances, &instance_count );

			// Set if dirty
			if ( set_shader != shader )
			{
				m_device_context->CSSetShader( shader, nullptr, 0 );
			}
		}

		// Render target(s)
		{
			// Current state
			core::ARRAY<ID3D11RenderTargetView*, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> set_render_target_views;
			ID3D11DepthStencilView* set_depth_stencil_view;
			m_device_context->OMGetRenderTargets( D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, set_render_target_views.data(), &set_depth_stencil_view );

			// Set if dirty
			if ( pipeline_state.m_render_targets != set_render_target_views )
			{
				UINT render_target_count = 0;
				for ( ID3D11RenderTargetView* rt : pipeline_state.m_render_targets )
				{
					if ( rt )
					{
						render_target_count++;
					}
				}

				m_device_context->OMSetRenderTargets
				(
					render_target_count,
					reinterpret_cast<ID3D11RenderTargetView* const*>(pipeline_state.m_render_targets.data()),
					nullptr
				);
			}
		}

		// Rasterizer state
		{
			// New state
			ID3D11RasterizerState* rasterizer_state = static_cast<ID3D11RasterizerState*>(pipeline_state.m_rasteriser_state ? pipeline_state.m_rasteriser_state->get_rasterizer_state() : nullptr);

			// Current state
			ID3D11RasterizerState* rasterizer_state_set = nullptr;

			// Set if dirty
			m_device_context->RSGetState( &rasterizer_state_set );
			if ( rasterizer_state_set != rasterizer_state )
			{
				m_device_context->RSSetState( rasterizer_state );
			}
		}

		// Primitive topology
		if ( pipeline_state.primitive_topology != RHI_PrimitiveTopology_Unknown )
		{
			// New state
			const D3D11_PRIMITIVE_TOPOLOGY topology = d3d11_primitive_topology[pipeline_state.primitive_topology];

			// Current state
			D3D11_PRIMITIVE_TOPOLOGY topology_set;
			m_device_context->IAGetPrimitiveTopology( &topology_set );

			// Set if dirty
			if ( topology_set != topology )
			{
				m_device_context->IASetPrimitiveTopology( topology );
			}
		}

		// Viewport
		if ( pipeline_state.m_viewport.is_valid() )
		{
			set_viewport( pipeline_state.m_viewport );
		}

		return true;
	}

	void RENDERING_CONTEXT_D3D11::clear_render_target_view( ID3D11RenderTargetView* render_target_view, const core::VECTOR3D<float>& rgb )
	{
		const float colour[] { rgb.x, rgb.y, rgb.z, 1.0f };
		m_device_context->ClearRenderTargetView( render_target_view, colour );
	}

	void RENDERING_CONTEXT_D3D11::set_viewport( const VIEWPORT& viewport ) const
	{
		const auto top_left = viewport.m_area.get_top_left();

		D3D11_VIEWPORT d3d11_viewport;
		d3d11_viewport.TopLeftX = top_left.x;
		d3d11_viewport.TopLeftY = top_left.y;
		d3d11_viewport.Width = viewport.m_area.get_width();
		d3d11_viewport.Height = viewport.m_area.get_height();
		d3d11_viewport.MinDepth = viewport.m_min_depth;
		d3d11_viewport.MaxDepth = viewport.m_max_depth;

		m_device_context->RSSetViewports( 1, &d3d11_viewport );
	}

	void RENDERING_CONTEXT_D3D11::set_vertex_buffer( BUFFER_D3D11& buffer_d3d11, uint32_t offset )
	{
		ID3D11Buffer* vertex_buffer = buffer_d3d11.GetResource();

		if ( !vertex_buffer )
		{
			ASSERT_SHOULDNT_BE_HERE();
			return;
		}

		UINT stride = buffer_d3d11.GetStride();
		UINT offsets = offset;

		// Get currently set buffer
		ID3D11Buffer* set_buffer = nullptr;
		UINT set_stride = buffer_d3d11.GetStride();
		UINT set_offset = 0;
		m_device_context->IAGetVertexBuffers( 0, 1, &set_buffer, &set_stride, &set_offset );

		// Skip if the same vertex buffer is already set
		if ( set_buffer != vertex_buffer || set_offset != offset )
		{
			m_device_context->IASetVertexBuffers( 0, 1, &vertex_buffer, &stride, &offsets );
		}
	}

	void RENDERING_CONTEXT_D3D11::set_index_buffer( BUFFER_D3D11& buffer_d3d11, uint32_t offset )
	{
		ID3D11Buffer* index_buffer = buffer_d3d11.GetResource();

		if ( !index_buffer )
		{
			ASSERT_SHOULDNT_BE_HERE();
			return;
		}

		const DXGI_FORMAT format = DXGI_FORMAT_R32_UINT;// buffer_d3d11->Is16Bit() ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

		// Get currently set buffer
		ID3D11Buffer* set_buffer = nullptr;
		DXGI_FORMAT set_format = DXGI_FORMAT_UNKNOWN;
		UINT set_offset = 0;
		m_device_context->IAGetIndexBuffer( &set_buffer, &set_format, &set_offset );

		// Skip if already set
		if ( set_buffer != index_buffer || set_offset != offset )
		{
			m_device_context->IASetIndexBuffer( index_buffer, format, static_cast<UINT>(offset) );
		}
	}

	void RENDERING_CONTEXT_D3D11::set_constant_buffer( const uint32_t slot, const uint8_t shader_types, const BUFFER_D3D11& buffer_d3d11 )
	{
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(buffer_d3d11.GetResource());
		constexpr uint32_t num_buffers{ 1 };

		if ( shader_types & (uint8_t)RHI_Shader_Type::RHI_Shader_Vertex )
		{
			// Set only if not set
			ID3D11Buffer* set_buffer = nullptr;
			m_device_context->VSGetConstantBuffers( slot, num_buffers, &set_buffer );
			if ( set_buffer != buffer )
			{
				m_device_context->VSSetConstantBuffers( slot, num_buffers, static_cast<ID3D11Buffer* const*>(&buffer) );
			}
		}

		if ( shader_types & (uint8_t)RHI_Shader_Type::RHI_Shader_Pixel )
		{
			// Set only if not set
			ID3D11Buffer* set_buffer = nullptr;
			m_device_context->PSGetConstantBuffers( slot, num_buffers, &set_buffer );
			if ( set_buffer != buffer )
			{
				m_device_context->PSSetConstantBuffers( slot, num_buffers, static_cast<ID3D11Buffer* const*>(&buffer) );
			}
		}

		if ( shader_types & (uint8_t)RHI_Shader_Type::RHI_Shader_Compute )
		{
			// Set only if not set
			ID3D11Buffer* set_buffer = nullptr;
			m_device_context->CSGetConstantBuffers( slot, num_buffers, &set_buffer );
			if ( set_buffer != buffer )
			{
				m_device_context->CSSetConstantBuffers( slot, num_buffers, static_cast<ID3D11Buffer* const*>(&buffer) );
			}
		}
	}

	void RENDERING_CONTEXT_D3D11::set_structured_buffer_uav( const uint32_t slot, const STRUCTURED_BUFFER_UAV_D3D11& structured_buffer ) const
    {
        std::array<void*, 1> view_array = { structured_buffer.get_resource_uav() };
        const UINT range = 1;

        ID3D11UnorderedAccessView* set_uav{};
        m_device_context->CSGetUnorderedAccessViews( slot, range, &set_uav );

        if ( set_uav != view_array[ 0 ] )
        {
            m_device_context->CSSetUnorderedAccessViews( slot, range, reinterpret_cast<ID3D11UnorderedAccessView* const*>( &view_array ), nullptr );
        }
    }

	void RENDERING_CONTEXT_D3D11::set_structured_buffer_srv( const uint32_t slot, const STRUCTURED_BUFFER_SRV_D3D11& structured_buffer ) const
    {
        std::array<void*, 1> view_array = { structured_buffer.get_resource_srv() };
        const UINT range = 1;

        ID3D11ShaderResourceView* set_uav{};
        m_device_context->CSGetShaderResources( slot, range, &set_uav );

        if ( set_uav != view_array[ 0 ] )
        {
            m_device_context->CSSetShaderResources( slot, range, reinterpret_cast<ID3D11ShaderResourceView* const*>( &view_array ) );
        }
    }

	void RENDERING_CONTEXT_D3D11::set_texture( const TEXTURE_SLOTS texture_slot, TEXTURE2D_D3D11* texture )
	{
		ASSERT( texture, "Texture is nullptr" );

		const auto start_slot{ static_cast<UINT>(texture_slot) };
		const UINT range = 1;

		const void* srv_array[1] = { texture ? texture->get_resource_view() : nullptr };
		ID3D11ShaderResourceView* set_srv = nullptr;
		m_device_context->PSGetShaderResources( start_slot, range, &set_srv );
		if ( set_srv != srv_array[0] )
		{
			m_device_context->PSSetShaderResources( start_slot, range, reinterpret_cast<ID3D11ShaderResourceView* const*>(&srv_array) );
		}
	}

	void RENDERING_CONTEXT_D3D11::set_sampler( SAMPLER_SLOTS sampler_slot, SAMPLER_D3D11* sampler )
	{
		const auto start_slot{ static_cast<UINT>(sampler_slot) };
		const UINT range = 1;
		const void* sampler_array[1] = { sampler ? sampler->GetResource() : nullptr };
		
		ID3D11SamplerState* set_sampler;
		m_device_context->PSGetSamplers( start_slot, range, &set_sampler );
		if ( set_sampler != sampler_array[0] )
		{
			m_device_context->PSSetSamplers( start_slot, range, reinterpret_cast<ID3D11SamplerState* const*>(&sampler_array) );
		}
	}

	void RENDERING_CONTEXT_D3D11::set_scissor_rect( const core::FRECTANGLE& rect )
	{
		const D3D11_RECT d3d11_rectangle = { static_cast<LONG>(rect.get_left()), static_cast<LONG>(rect.get_top()), 
			static_cast<LONG>(rect.get_right()), static_cast<LONG>(rect.get_bottom()) };

		m_device_context->RSSetScissorRects( 1u, &d3d11_rectangle );
	}

	bool RENDERING_CONTEXT_D3D11::draw( const uint32_t vertex_count, const uint32_t vertex_offset )
	{
		m_device_context->Draw( vertex_count, vertex_offset );

		return true;
	}

	bool RENDERING_CONTEXT_D3D11::draw_indexed( const uint32_t index_count, const uint32_t index_offset, const uint32_t vertex_offset )
	{
		m_device_context->DrawIndexed( static_cast<UINT>(index_count), static_cast<UINT>(index_offset), static_cast<INT>(vertex_offset) );

		return true;
	}

	void RENDERING_CONTEXT_D3D11::dispatch_compute( const uint32_t x, const uint32_t y, const uint32_t z )
	{
		m_device_context->Dispatch( x, y, z ); // thread group count will be the max of the numbers pass here and the const numbers in the compute shader

		// Make sure to clean the compute shader UAV slots after dispatching.
        // If we try to bind the resource but it's still bound as a computer shader output the runtime will automatically set the it to null.
		constexpr auto NUM_RESOURCES{ 8u };
        const void* resource_array[ NUM_RESOURCES ] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
        m_device_context->CSSetUnorderedAccessViews( 0u, NUM_RESOURCES, reinterpret_cast<ID3D11UnorderedAccessView* const*>( &resource_array ), nullptr );
	}
}
