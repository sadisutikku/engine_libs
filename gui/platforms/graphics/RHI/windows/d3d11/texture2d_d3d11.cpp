
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/texture2d_d3d11.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/d3d11_types.h"
#include "gui/platforms/graphics/renderer/windows/renderer_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"
#include "gui/shared/graphics/RHI/RHI_types.h"

#include <d3d11.h>

namespace
{
	inline UINT GetBindFlags( uint16_t flags )
	{
		UINT flags_d3d11 = 0;

		flags_d3d11 |= (flags & gui::RHI_Texture_Sampled) ? D3D11_BIND_SHADER_RESOURCE : 0;
		flags_d3d11 |= (flags & gui::RHI_Texture_Storage) ? D3D11_BIND_UNORDERED_ACCESS : 0;
		flags_d3d11 |= (flags & gui::RHI_Texture_DepthStencil) ? D3D11_BIND_DEPTH_STENCIL : 0;
		flags_d3d11 |= (flags & gui::RHI_Texture_RenderTarget) ? D3D11_BIND_RENDER_TARGET : 0;

		return flags_d3d11;
	}

	inline DXGI_FORMAT GetDepthFormat( gui::RHI_Format format )
	{
		if ( format == gui::RHI_Format_D32_Float_S8X24_Uint )
		{
			return DXGI_FORMAT_R32G8X24_TYPELESS;
		}
		else if ( format == gui::RHI_Format_D32_Float )
		{
			return DXGI_FORMAT_R32_TYPELESS;
		}

		return gui::d3d11_format[format];
	}

	inline DXGI_FORMAT GetDepthFormatDsv( gui::RHI_Format format )
	{
		if ( format == gui::RHI_Format_D32_Float_S8X24_Uint )
		{
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		}
		else if ( format == gui::RHI_Format_D32_Float )
		{
			return DXGI_FORMAT_D32_FLOAT;
		}

		return gui::d3d11_format[format];
	}

	inline DXGI_FORMAT GetDepthFormatSrv( gui::RHI_Format format )
	{
		if ( format == gui::RHI_Format_D32_Float_S8X24_Uint )
		{
			return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		}
		else if ( format == gui::RHI_Format_D32_Float )
		{
			return DXGI_FORMAT_R32_FLOAT;
		}

		return gui::d3d11_format[format];
	}

	inline bool CreateTexture2d(
		void*& texture,
		const uint32_t width,
		const uint32_t height,
		const uint32_t channels,
		const uint32_t bits_per_channel,
		const uint32_t array_size,
		const uint8_t mip_count,
		const DXGI_FORMAT format,
		const UINT bind_flags,
		std::vector< std::vector<std::byte> >& data,
		gui::RENDERER_D3D11& renderer
	)
	{
		const bool has_initial_data = data.size() == mip_count;

		// Describe
		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = static_cast<UINT>(width);
		texture_desc.Height = static_cast<UINT>(height);
		texture_desc.MipLevels = mip_count;
		texture_desc.ArraySize = static_cast<UINT>(array_size);
		texture_desc.Format = format;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = has_initial_data ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = bind_flags;
		texture_desc.MiscFlags = 0;
		texture_desc.CPUAccessFlags = 0;

		// Set initial data
		std::vector<D3D11_SUBRESOURCE_DATA> vec_subresource_data;
		if ( has_initial_data )
		{
			for ( uint8_t i = 0; i < mip_count; i++ )
			{
				D3D11_SUBRESOURCE_DATA& subresource_data = vec_subresource_data.emplace_back( D3D11_SUBRESOURCE_DATA{} );
				subresource_data.pSysMem = i < data.size() ? data[i].data() : nullptr;        // Data pointer
				subresource_data.SysMemPitch = (width >> i) * channels * (bits_per_channel / 8); // Line width in bytes
				subresource_data.SysMemSlicePitch = 0;                                                // This is only used for 3D textures
			}
		}

		const auto result = renderer.get_device().get_device()->CreateTexture2D( &texture_desc, vec_subresource_data.data(), reinterpret_cast<ID3D11Texture2D**>(&texture) );
		ASSERT( SUCCEEDED( result ), "Call to CreateRenderTargetView failed" );

		return SUCCEEDED( result );
	}

	inline bool CreateRenderTargetView2d( void* texture, std::array<void*, gui::rhi_max_render_target_count>& views,
										  const DXGI_FORMAT format, const unsigned array_size, gui::RENDERER_D3D11& renderer )
	{
		// Describe
		D3D11_RENDER_TARGET_VIEW_DESC view_desc = {};
		view_desc.Format = format;
		view_desc.ViewDimension = (array_size == 1) ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		view_desc.Texture2DArray.MipSlice = 0;
		view_desc.Texture2DArray.ArraySize = 1;
		view_desc.Texture2DArray.FirstArraySlice = 0;

		// Create
		for ( uint32_t i = 0; i < array_size; i++ )
		{
			view_desc.Texture2DArray.FirstArraySlice = i;
			const auto result = renderer.get_device().get_device()->CreateRenderTargetView( static_cast<ID3D11Resource*>(texture), &view_desc, reinterpret_cast<ID3D11RenderTargetView**>(&views[i]) );
			if ( FAILED( result ) )
			{
				ASSERT_FAILED( "Call to CreateRenderTargetView failed" );
				//LOG_ERROR( "Failed, %s.", d3d11_utility::dxgi_error_to_string( result ) );
				return false;
			}
		}

		return true;
	}

	inline bool CreateDepthStencilView2d( void* texture, std::array<void*, gui::rhi_max_render_target_count>& views, 
										  const uint32_t array_size, const DXGI_FORMAT format, bool read_only, gui::RENDERER_D3D11& renderer )
	{
		// Describe
		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		dsv_desc.Format = format;
		dsv_desc.ViewDimension = (array_size == 1) ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsv_desc.Texture2DArray.MipSlice = 0;
		dsv_desc.Texture2DArray.ArraySize = 1;
		dsv_desc.Texture2DArray.FirstArraySlice = 0;
		dsv_desc.Flags = read_only ? D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL : 0;

		// Create
		for ( uint32_t i = 0; i < array_size; i++ )
		{
			dsv_desc.Texture2DArray.FirstArraySlice = i;
			const auto result = renderer.get_device().get_device()->CreateDepthStencilView( static_cast<ID3D11Resource*>(texture), &dsv_desc, reinterpret_cast<ID3D11DepthStencilView**>(&views[i]) );
			if ( FAILED( result ) )
			{
				ASSERT_FAILED( "Call to CreateDepthStencilView failed" );
				return false;
			}
		}

		return true;
	}

	inline bool CreateShaderResourceView2d( void* texture, void*& view, DXGI_FORMAT format, uint32_t array_size, 
											std::vector< std::vector<std::byte> >& data, gui::RENDERER_D3D11& renderer )
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
		shader_resource_view_desc.Format = format;
		shader_resource_view_desc.ViewDimension = (array_size == 1) ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		shader_resource_view_desc.Texture2DArray.FirstArraySlice = 0;
		shader_resource_view_desc.Texture2DArray.MostDetailedMip = 0;
		shader_resource_view_desc.Texture2DArray.MipLevels = data.empty() ? 1 : static_cast<UINT>(data.size());
		shader_resource_view_desc.Texture2DArray.ArraySize = array_size;
				
		const auto result = renderer.get_device().get_device()->CreateShaderResourceView( static_cast<ID3D11Resource*>(texture), &shader_resource_view_desc, reinterpret_cast<ID3D11ShaderResourceView**>(&view) );
		ASSERT( SUCCEEDED( result ), "Call to CreateShaderResourceView failed" );
		return SUCCEEDED( result );
	}

	inline bool CreateUnorderedAccessView2d( void* texture, void*& view, DXGI_FORMAT format, int32_t array_size, gui::RENDERER_D3D11& renderer )
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC unorderd_access_view_desc = {};
		unorderd_access_view_desc.Format = format;
		unorderd_access_view_desc.ViewDimension = (array_size == 1) ? D3D11_UAV_DIMENSION_TEXTURE2D : D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
		unorderd_access_view_desc.Texture2DArray.FirstArraySlice = 0;
		unorderd_access_view_desc.Texture2DArray.ArraySize = array_size;

		const auto result = renderer.get_device().get_device()->CreateUnorderedAccessView( static_cast<ID3D11Resource*>(texture), &unorderd_access_view_desc, reinterpret_cast<ID3D11UnorderedAccessView**>(&view) );
		ASSERT( SUCCEEDED( result ), "Call to CreateUnorderedAccessView failed" );
		return SUCCEEDED( result );
	}
}

namespace gui
{
	TEXTURE2D_D3D11::TEXTURE2D_D3D11( RENDERER_D3D11& renderer, const uint32_t width, const uint32_t height, const RHI_Format format, const uint16_t flags, const std::vector<std::byte>& data )
		: TEXTURE2D_BASE( width, height, format, flags, data )
	{
		create_texture2d_d3d11( renderer );
	}

	bool TEXTURE2D_D3D11::create_texture2d_d3d11( RENDERER_D3D11& renderer )
	{
		bool result_tex = true;
		bool result_srv = true;
		bool result_uav = true;
		bool result_rt = true;
		bool result_ds = true;

		// Get texture flags
		const UINT flags = GetBindFlags( m_flags );

		// Resolve formats
		const DXGI_FORMAT format = GetDepthFormat( m_format );
		const DXGI_FORMAT format_dsv = GetDepthFormatDsv( m_format );
		const DXGI_FORMAT format_srv = GetDepthFormatSrv( m_format );

		// TEXTURE
		result_tex = CreateTexture2d
		(
			m_resource,
			m_width,
			m_height,
			m_channel_count,
			m_bits_per_channel,
			m_array_size,
			m_mip_count,
			format,
			flags,
			m_data,
			renderer
		);

		// RESOURCE VIEW
		if ( IsSampled() )
		{
			result_srv = CreateShaderResourceView2d(
				m_resource,
				m_resource_view[0],
				format_srv,
				m_array_size,
				m_data,
				renderer
			);
		}

		// UNORDERED ACCESS VIEW
		if ( IsStorage() )
		{
			result_uav = CreateUnorderedAccessView2d( m_resource, m_resource_view_unorderedAccess, format, m_array_size, renderer );
		}

		// DEPTH-STENCIL VIEW
		if ( IsDepthStencil() )
		{
			result_ds = CreateDepthStencilView2d
			(
				m_resource,
				m_resource_view_depthStencil,
				m_array_size,
				format_dsv,
				false,
				renderer
			);

			if ( m_flags & RHI_Texture_DepthStencilReadOnly )
			{
				result_ds = CreateDepthStencilView2d
				(
					m_resource,
					m_resource_view_depthStencilReadOnly,
					m_array_size,
					format_dsv,
					true,
					renderer
				);
			}
		}

		// RENDER TARGET VIEW
		if ( IsRenderTarget() )
		{
			result_rt = CreateRenderTargetView2d
			(
				m_resource,
				m_resource_view_renderTarget,
				format,
				m_array_size,
				renderer
			);
		}

		ASSERT( result_tex && result_srv && result_uav && result_rt && result_ds, "Failed to create texture2d" );

		return result_tex && result_srv && result_uav && result_rt && result_ds;
	}
}
