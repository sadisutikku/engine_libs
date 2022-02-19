

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_swapchain_d3d11.h"

#include "core/shared/debugging/asserts.h"
#include "core/shared/math/vector3d.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_context_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/d3d11_types.h"

#include <dxgi1_5.h>
#include <dxgiformat.h>

namespace
{
	// Determines whether tearing support is available for fullscreen borderless windows.
	inline bool CheckTearingSupport( const gui::RENDERING_DEVICE_D3D11& device )
	{
		// Rather than create the 1.5 factory interface directly, we create the 1.4
		// interface and query for the 1.5 interface. This will enable the graphics
		// debugging tools which might not support the 1.5 factory interface
		wrl::ComPtr<IDXGIFactory4> factory4;
		HRESULT resut = CreateDXGIFactory1( IID_PPV_ARGS( factory4.GetAddressOf() ) );
		BOOL allowTearing = FALSE;
		if ( SUCCEEDED( resut ) )
		{
			wrl::ComPtr<IDXGIFactory5> factory5;
			resut = factory4.As( &factory5 );
			if ( SUCCEEDED( resut ) )
			{
				resut = factory5->CheckFeatureSupport( DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof( allowTearing ) );
			}
		}

		const bool fullscreen_borderless_support = SUCCEEDED( resut ) && allowTearing;
		const bool vendor_support = !device.GetPrimaryPhysicalDevice()->IsIntel(); // Intel, bad

		return fullscreen_borderless_support && vendor_support;
	}

	inline uint32_t validate_flags( const gui::RENDERING_DEVICE_D3D11& device, uint32_t flags )
	{
		// If SwapChain_Allow_Tearing was requested
		if ( flags & gui::RHI_Present_Mode::RHI_Present_Immediate )
		{
			// Check if the adapter supports it, if not, disable it (tends to fail with Intel adapters)
			if ( !CheckTearingSupport( device ) )
			{
				flags &= ~gui::RHI_Present_Immediate;
				//LOG_WARNING( "Present_Immediate was requested but it's not supported by the adapter." );
			}
		}

		return flags;
	}

	inline UINT get_d3d11_flags( uint32_t flags )
	{
		UINT d3d11_flags = 0;

		d3d11_flags |= flags & gui::RHI_SwapChain_Allow_Mode_Switch ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;
		d3d11_flags |= flags & gui::RHI_Present_Immediate ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		return d3d11_flags;
	}

	inline DXGI_SWAP_EFFECT get_swap_effect( const gui::RENDERING_DEVICE_D3D11& device, uint32_t flags )
	{
#if !defined(_WIN32_WINNT_WIN10)
		if ( flags & RHI_Swap_Flip_Discard )
		{
			//LOG_WARNING( "Swap_Flip_Discard was requested but it's only supported in Windows 10, using Swap_Discard instead." );
			flags &= ~RHI_Swap_Flip_Discard;
			flags |= RHI_Swap_Discard;
		}
#endif

		if ( flags & gui::RHI_Swap_Flip_Discard && device.GetPrimaryPhysicalDevice()->IsIntel() )
		{
			//LOG_WARNING( "Swap_Flip_Discard was requested but it's not supported by Intel adapters, using Swap_Discard instead." );
			flags &= ~gui::RHI_Swap_Flip_Discard;
			flags |= gui::RHI_Swap_Discard;
		}

		if ( flags & gui::RHI_Swap_Discard )           return DXGI_SWAP_EFFECT_DISCARD;
		if ( flags & gui::RHI_Swap_Sequential )        return DXGI_SWAP_EFFECT_SEQUENTIAL;
		if ( flags & gui::RHI_Swap_Flip_Discard )      return DXGI_SWAP_EFFECT_FLIP_DISCARD;
		if ( flags & gui::RHI_Swap_Flip_Sequential )   return DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		ASSERT_FAILED( "Unable to determine the requested swap effect, opting for DXGI_SWAP_EFFECT_DISCARD" );
		return DXGI_SWAP_EFFECT_DISCARD;
	}
}

namespace gui
{
	SWAPCHAIN_D3D11::SWAPCHAIN_D3D11( HWND window_handle, RENDERING_DEVICE_D3D11& rhi_device, const float width, const float height,
		RHI_Format format, uint32_t flags, const char* name )
		: m_device{ rhi_device }
		, m_viewport_quad{ m_device, { width, height } }
	{
		// Validate device
		if ( !m_device.get_device() )
		{
			ASSERT_FAILED( "Invalid device." );
			return;
		}

		// Validate window handle
		if ( !window_handle || !IsWindow( window_handle ) )
		{
			ASSERT_FAILED( "Invalid window." );
			return;
		}

		// Validate resolution
		if ( !m_device.ValidateResolution( static_cast<uint32_t>(width), static_cast<uint32_t>(height) ) )
		{
			//LOG_WARNING( "%dx%d is an invalid resolution", width, height );
			return;
		}

		// Get factory
		wrl::ComPtr<IDXGIFactory> dxgi_factory;
		if ( const auto& adapter = m_device.GetPrimaryPhysicalDevice() )
		{
			auto dxgi_adapter = static_cast<IDXGIAdapter*>(adapter->GetData());
			if ( dxgi_adapter->GetParent( IID_PPV_ARGS( dxgi_factory.GetAddressOf() ) ) != S_OK )
			{
				ASSERT_FAILED( "Failed to get adapter's factory" );
				return;
			}
		}
		else
		{
			ASSERT_FAILED( "Invalid primary adapter" );
			return;
		}

		m_format = format;
		m_buffer_count = s_swap_chain_buffer_count;
		m_windowed = true;
		m_dimensions = { width, height };
		m_flags = validate_flags( m_device, flags );

		// Create swap chain
		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferCount = static_cast<UINT>(m_buffer_count);
		desc.BufferDesc.Width = static_cast<UINT>(width);
		desc.BufferDesc.Height = static_cast<UINT>(height);
		desc.BufferDesc.Format = d3d11_format[format];
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = window_handle;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Windowed = m_windowed;
		desc.SwapEffect = get_swap_effect( m_device, m_flags );
		desc.Flags = get_d3d11_flags( m_flags );

		if ( FAILED( dxgi_factory->CreateSwapChain( m_device.get_device(), &desc, m_swapchain.GetAddressOf() ) ) )
		{
			ASSERT_FAILED( "Failed to create swapchain" );
			return;
		}

		// Create the render target
		wrl::ComPtr<ID3D11Texture2D> backbuffer;
		auto result = m_swapchain->GetBuffer( 0, IID_PPV_ARGS( backbuffer.GetAddressOf() ) );
		if ( FAILED( result ) )
		{
			ASSERT_FAILED( "Failed to get swapchain buffer" );
			return;
		}

		result = m_device.get_device()->CreateRenderTargetView( backbuffer.Get(), nullptr, m_render_target_view.GetAddressOf() );
		if ( FAILED( result ) )
		{
			ASSERT_FAILED( "Call to CreateRenderTargetView failed" );
			return;
		}

		// Create command lists
		for ( uint32_t i = 0; i < m_buffer_count; ++i )
		{
			//m_cmd_lists.emplace_back( make_shared<RHI_CommandList>( i, this, rhi_device->get_context()->device_context ) );
		}

		//m_cmd_index = 0;
	}

	bool SWAPCHAIN_D3D11::resize( const float width, const float height, const bool force /*= false*/ )
	{
		if ( !m_swapchain )
		{
			ASSERT_SHOULDNT_BE_HERE();
			return false;
		}

		// Validate resolution
		const bool present_enabled = m_device.ValidateResolution( static_cast<uint32_t>(width), static_cast<uint32_t>(height) );
		if ( !present_enabled )
		{
			// Return true as when minimizing, a resolution
			// of 0,0 can be passed in, and this is fine.
			return true;
		}

		// Only resize if needed
		if ( !force && m_dimensions.x == width && m_dimensions.y == height )
		{
			return true;
		}

		// Save new dimensions
		m_dimensions = { width, height };

		m_render_target_view->Release(); // release previous render target view

		// Set this flag to enable an application to switch modes by calling IDXGISwapChain::ResizeTarget.
		// When switching from windowed to full-screen mode, the display mode (or monitor resolution)
		// will be changed to match the dimensions of the application window.
		//if ( m_flags & RHI_SwapChain_Allow_Mode_Switch )
		//{
		//	const DisplayMode& display_mode = Display::GetActiveDisplayMode();

		//	// Resize swapchain target
		//	DXGI_MODE_DESC dxgi_mode_desc = {};
		//	dxgi_mode_desc.Width = static_cast<UINT>(width);
		//	dxgi_mode_desc.Height = static_cast<UINT>(height);
		//	dxgi_mode_desc.Format = d3d11_format[m_format];
		//	dxgi_mode_desc.RefreshRate = DXGI_RATIONAL{ display_mode.numerator, display_mode.denominator };
		//	dxgi_mode_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		//	dxgi_mode_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		//	// Resize swapchain target
		//	const auto result = swap_chain->ResizeTarget( &dxgi_mode_desc );
		//	if ( FAILED( result ) )
		//	{
		//		LOG_ERROR( "Failed to resize swapchain target, %s.", d3d11_utility::dxgi_error_to_string( result ) );
		//		return false;
		//	}
		//}

		// Resize swapchain buffers
		const UINT d3d11_flags = get_d3d11_flags( validate_flags( m_device, m_flags ) );
		auto result = m_swapchain->ResizeBuffers( m_buffer_count, static_cast<UINT>(width), static_cast<UINT>(height), d3d11_format[m_format], d3d11_flags );
		if ( FAILED( result ) )
		{
			ASSERT_FAILED( "Failed to resize swapchain buffers" );
			return false;
		}

		// Get swapchain back-buffer
		wrl::ComPtr<ID3D11Texture2D> backbuffer;
		result = m_swapchain->GetBuffer( 0, IID_PPV_ARGS( backbuffer.GetAddressOf() ) );
		if ( FAILED( result ) )
		{
			ASSERT_FAILED( "Failed to get swapchain buffer" );
			return false;
		}

		// Create render target view
		result = m_device.get_device()->CreateRenderTargetView( backbuffer.Get(), nullptr, m_render_target_view.GetAddressOf() );
		if ( FAILED( result ) )
		{
			ASSERT_FAILED( "Failed to create render target view" );
			return false;
		}

		return true;
	}

	void SWAPCHAIN_D3D11::clear_backbuffer( const core::VECTOR3D<float>& rgb )
	{
		m_device.get_context()->clear_render_target_view( m_render_target_view.Get(), rgb );
	}

	void SWAPCHAIN_D3D11::present()
	{
		m_swapchain->Present( 1u, 0u );
	}
}
