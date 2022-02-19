
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_context_d3d11.h"

#include <array>
#include <dxgi.h>

namespace
{
	inline const char* dxgi_error_to_string( const HRESULT error_code )
	{
		switch ( error_code )
		{
		case DXGI_ERROR_DEVICE_HUNG:                    return "DXGI_ERROR_DEVICE_HUNG";                    // The application's device failed due to badly formed commands sent by the application. This is an design-time issue that should be investigated and fixed.
		case DXGI_ERROR_DEVICE_REMOVED:                 return "DXGI_ERROR_DEVICE_REMOVED";                 // The video card has been physically removed from the system, or a driver upgrade for the video card has occurred. The application should destroy and recreate the device. For help debugging the problem, call ID3D10Device::GetDeviceRemovedReason.
		case DXGI_ERROR_DEVICE_RESET:                   return "DXGI_ERROR_DEVICE_RESET";                   // The device failed due to a badly formed command. This is a run-time issue; The application should destroy and recreate the device.
		case DXGI_ERROR_DRIVER_INTERNAL_ERROR:          return "DXGI_ERROR_DRIVER_INTERNAL_ERROR";          // The driver encountered a problem and was put into the device removed state.
		case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:      return "DXGI_ERROR_FRAME_STATISTICS_DISJOINT";      // An event (for example, a power cycle) interrupted the gathering of presentation statistics.
		case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE:   return "DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE";   // The application attempted to acquire exclusive ownership of an output, but failed because some other application (or device within the application) already acquired ownership.
		case DXGI_ERROR_INVALID_CALL:                   return "DXGI_ERROR_INVALID_CALL";                   // The application provided invalid parameter data; this must be debugged and fixed before the application is released.
		case DXGI_ERROR_MORE_DATA:                      return "DXGI_ERROR_MORE_DATA";                      // The buffer supplied by the application is not big enough to hold the requested data.
		case DXGI_ERROR_NONEXCLUSIVE:                   return "DXGI_ERROR_NONEXCLUSIVE";                   // A global counter resource is in use, and the Direct3D device can't currently use the counter resource.
		case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:        return "DXGI_ERROR_NOT_CURRENTLY_AVAILABLE";        // The resource or request is not currently available, but it might become available later.
		case DXGI_ERROR_NOT_FOUND:                      return "DXGI_ERROR_NOT_FOUND";                      // When calling IDXGIObject::GetPrivateData, the GUID passed in is not recognized as one previously passed to IDXGIObject::SetPrivateData or IDXGIObject::SetPrivateDataInterface. When calling IDXGIFentityy::EnumAdapters or IDXGIAdapter::EnumOutputs, the enumerated ordinal is out of range.
		case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED:     return "DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED";     // Reserved
		case DXGI_ERROR_REMOTE_OUTOFMEMORY:             return "DXGI_ERROR_REMOTE_OUTOFMEMORY";             // Reserved
		case DXGI_ERROR_WAS_STILL_DRAWING:              return "DXGI_ERROR_WAS_STILL_DRAWING";              // The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.
		case DXGI_ERROR_UNSUPPORTED:                    return "DXGI_ERROR_UNSUPPORTED";                    // The requested functionality is not supported by the device or the driver.
		case DXGI_ERROR_ACCESS_LOST:                    return "DXGI_ERROR_ACCESS_LOST";                    // The desktop duplication interface is invalid. The desktop duplication interface typically becomes invalid when a different type of image is displayed on the desktop.
		case DXGI_ERROR_WAIT_TIMEOUT:                   return "DXGI_ERROR_WAIT_TIMEOUT";                   // The time-out interval elapsed before the next desktop frame was available.
		case DXGI_ERROR_SESSION_DISCONNECTED:           return "DXGI_ERROR_SESSION_DISCONNECTED";           // The Remote Desktop Services session is currently disconnected.
		case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE:       return "DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE";       // The DXGI output (monitor) to which the swap chain content was restricted is now disconnected or changed.
		case DXGI_ERROR_CANNOT_PROTECT_CONTENT:         return "DXGI_ERROR_CANNOT_PROTECT_CONTENT";         // DXGI can't provide content protection on the swap chain. This error is typically caused by an older driver, or when you use a swap chain that is incompatible with content protection.
		case DXGI_ERROR_ACCESS_DENIED:                  return "DXGI_ERROR_ACCESS_DENIED";                  // You tried to use a resource to which you did not have the required access privileges. This error is most typically caused when you write to a shared resource with read-only access.
		case DXGI_ERROR_NAME_ALREADY_EXISTS:            return "DXGI_ERROR_NAME_ALREADY_EXISTS";            // The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle is already associated with some other resource.
		case DXGI_ERROR_SDK_COMPONENT_MISSING:          return "DXGI_ERROR_SDK_COMPONENT_MISSING";          // The application requested an operation that depends on an SDK component that is missing or mismatched.
		case DXGI_ERROR_NOT_CURRENT:                    return "DXGI_ERROR_NOT_CURRENT";                    // The DXGI objects that the application has created are no longer current & need to be recreated for this operation to be performed.
		case DXGI_ERROR_HW_PROTECTION_OUTOFMEMORY:      return "DXGI_ERROR_HW_PROTECTION_OUTOFMEMORY";      // Insufficient HW protected memory exits for proper function.
		case DXGI_ERROR_DYNAMIC_CODE_POLICY_VIOLATION:  return "DXGI_ERROR_DYNAMIC_CODE_POLICY_VIOLATION";  // Creating this device would violate the process's dynamic code policy.
		case DXGI_ERROR_NON_COMPOSITED_UI:              return "DXGI_ERROR_NON_COMPOSITED_UI";              // The operation failed because the compositor is not in control of the output.
		case E_INVALIDARG:                              return "E_INVALIDARG";                              // One or more arguments are invalid.
		}

		return "Unknown error code";
	}
}

namespace gui
{
	RENDERING_DEVICE_D3D11::RENDERING_DEVICE_D3D11()
		: m_context{ std::make_unique<RENDERING_CONTEXT_D3D11>() }
	{
		find_adapter();

		// Resource limits
		m_context->texture_2d_dimension_max = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;

		create_logical_device();

		constexpr bool multithread_protection{ true };
		if ( multithread_protection )
		{
			wrl::ComPtr<ID3D11Multithread> multithread = nullptr;
			if ( SUCCEEDED( m_context->m_device_context->QueryInterface( __uuidof(ID3D11Multithread), reinterpret_cast<void**>(multithread.GetAddressOf()) ) ) )
			{
				multithread->SetMultithreadProtected( TRUE );
			}
			else
			{
				ASSERT_FAILED( "Failed to enable multi-threaded protection" );
			}
		}

		// Annotations
		if ( m_context->debug )
		{
			const auto result = m_context->m_device_context->QueryInterface( IID_PPV_ARGS( m_context->m_annotation.GetAddressOf() ) );
			if ( FAILED( result ) )
			{
				//ASSERT_FAILED( "Failed to create ID3DUserDefinedAnnotation for event reporting, %s.", dxgi_error_to_string( result ) );
				return;
			}
		}
	}

	RENDERING_DEVICE_D3D11::~RENDERING_DEVICE_D3D11() = default;

	void RENDERING_DEVICE_D3D11::find_adapter()
	{
		wrl::ComPtr<IDXGIFactory1> factory;
		if ( FAILED( CreateDXGIFactory1( IID_PPV_ARGS( factory.GetAddressOf() ) ) ) )
		{
			ASSERT_FAILED( "CreateDXGIFactory1 returned unsuccessful" );
			return;
		}

		const auto get_available_adapters = []( IDXGIFactory1* factory )
		{
			uint32_t i = 0;
			IDXGIAdapter* adapter;
			std::vector<IDXGIAdapter*> adapters;
			while ( factory->EnumAdapters( i, &adapter ) != DXGI_ERROR_NOT_FOUND )
			{
				adapters.emplace_back( adapter );
				++i;
			}

			return adapters;
		};

		// Get all available adapters
		auto adapters = get_available_adapters( factory.Get() );
		if ( adapters.empty() )
		{
			ASSERT_FAILED( "Couldn't find any adapters" );
			return;
		}

		// Save all available adapters
		DXGI_ADAPTER_DESC adapter_desc;
		for ( auto display_adapter : adapters )
		{
			if ( FAILED( display_adapter->GetDesc( &adapter_desc ) ) )
			{
				ASSERT_FAILED( "Failed to get adapter description" );
				continue;
			}

			// Of course it wouldn't be simple, lets convert the device name
			char name[128];
			auto def_char = ' ';
			WideCharToMultiByte( CP_ACP, 0, adapter_desc.Description, -1, name, 128, &def_char, nullptr );

			RegisterPhysicalDevice( PYHSICAL_DEVICE(
														 11 << 22,                                                   // api version
														 0,                                                          // driver version
														 adapter_desc.VendorId,                                      // vendor id
														 RHI_PhysicalDevice_Unknown,                                 // type
														 &name[0],                                                   // name
														 static_cast<uint32_t>(adapter_desc.DedicatedVideoMemory),   // memory
														 static_cast<void*>(display_adapter) )                        // data
			);
		}

		// DISPLAY MODES
		const auto get_display_modes = []( IDXGIAdapter* adapter, RHI_Format format )
		{
			// Enumerate the primary adapter output (monitor).
			wrl::ComPtr<IDXGIOutput> adapter_output = nullptr;
			bool result = SUCCEEDED( adapter->EnumOutputs( 0, adapter_output.GetAddressOf() ) );
			if ( result )
			{
				// Get supported display mode count
				UINT display_mode_count = 0;
				result = SUCCEEDED( adapter_output->GetDisplayModeList( d3d11_format[format], DXGI_ENUM_MODES_INTERLACED, &display_mode_count, nullptr ) );
				if ( result )
				{
					// Get display modes
					std::vector<DXGI_MODE_DESC> display_modes;
					display_modes.resize( display_mode_count );
					result = SUCCEEDED( adapter_output->GetDisplayModeList( d3d11_format[format], DXGI_ENUM_MODES_INTERLACED, &display_mode_count, &display_modes[0] ) );
					if ( result )
					{
						// Save all the display modes
						for ( const DXGI_MODE_DESC& mode : display_modes )
						{
							//Display::RegisterDisplayMode( DisplayMode( mode.Width, mode.Height, mode.RefreshRate.Numerator, mode.RefreshRate.Denominator ), globals::rhi_device->GetContext() );
						}
					}
				}
			}

			return result;
		};

		// Get display modes and set primary adapter
		for ( uint32_t device_index = 0u; device_index < GetPhysicalDevices().size(); ++device_index )
		{
			const PYHSICAL_DEVICE& physical_device = GetPhysicalDevices()[device_index];
			const auto dx_adapter = static_cast<IDXGIAdapter*>(physical_device.GetData());

			// Adapters are ordered by memory (descending), so stop on the first success
			const auto format = RHI_Format_R8G8B8A8_Unorm; // TODO: This must come from the swapchain
			if ( get_display_modes( dx_adapter, format ) )
			{
				SetPrimaryPhysicalDevice( device_index );
				return;
			}
			else
			{
				//ASSERT_FAILED( "Failed to get display modes for \"%s\".", physical_device.GetName().c_str() );
			}
		}

		// If we failed to detect any display modes but we have at least one adapter, use it.
		if ( !GetPhysicalDevices().empty() )
		{
			ASSERT_FAILED( "Failed to detect display modes for all physical devices, falling back to first available." );
			SetPrimaryPhysicalDevice( 0 );
		}
	}

	void RENDERING_DEVICE_D3D11::create_logical_device()
	{
		const PYHSICAL_DEVICE* physical_device = GetPrimaryPhysicalDevice();
		if ( !physical_device )
		{
			ASSERT_FAILED( "Failed to detect any devices" );
			return;
		}

		// Flags
		UINT device_flags = 0;
		// Enable debug layer
		if ( m_context->debug )
		{
			device_flags |= D3D11_CREATE_DEVICE_DEBUG;
		}

		auto adapter = static_cast<IDXGIAdapter*>(physical_device->GetData());
		auto driver_type = adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE;

		auto create_device = [this, &adapter, &driver_type, &device_flags]()
		{
			// The order of the feature levels that we'll try to create a device with
			constexpr std::array<D3D_FEATURE_LEVEL, 7> FEATURE_LEVELS =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};

			wrl::ComPtr<ID3D11Device> temp_device;
			wrl::ComPtr<ID3D11DeviceContext> temp_context;

			const HRESULT result = D3D11CreateDevice(
				adapter,                                    // pAdapter: If nullptr, the default adapter will be used
				driver_type,                                // DriverType
				nullptr,                                    // HMODULE: nullptr because DriverType = D3D_DRIVER_TYPE_HARDWARE
				device_flags,                                // Flags
				FEATURE_LEVELS.data(),                        // pFeatureLevels
				static_cast<UINT>(FEATURE_LEVELS.size()),    // FeatureLevels
				D3D11_SDK_VERSION,                            // SDKVersion
				temp_device.GetAddressOf(),                                // ppDevice
				nullptr,                                    // pFeatureLevel
				temp_context.GetAddressOf() );

			if ( SUCCEEDED( result ) )
			{
				// Query old device for newer interface.
				if ( FAILED( temp_device->QueryInterface( __uuidof(ID3D11Device5), (void**)m_device.GetAddressOf() ) ) )
				{
					return E_FAIL;
				}

				// Query old device context for newer interface.
				if ( FAILED( temp_context->QueryInterface( __uuidof(ID3D11DeviceContext4), (void**)m_context->m_device_context.GetAddressOf() ) ) )
				{
					return E_FAIL;
				}
			}

			return result;
		};

		// Create Direct3D device and Direct3D device context.
		auto result = create_device();

		// Using the D3D11_CREATE_DEVICE_DEBUG flag, requires the SDK to be installed, so try again without it
		if ( result == DXGI_ERROR_SDK_COMPONENT_MISSING )
		{
			//LOG_WARNING( "Failed to create device with D3D11_CREATE_DEVICE_DEBUG flags as it requires the DirectX SDK to be installed. Attempting to create a device without it." );
			device_flags &= ~D3D11_CREATE_DEVICE_DEBUG;
			result = create_device();
		}

		if ( FAILED( result ) )
		{
			//ASSERT_FAILED( "Failed to create device, %s.", dxgi_error_to_string( result ) );
			return;
		}
	}

	bool RENDERING_DEVICE_D3D11::ValidateResolution( const uint32_t width, const uint32_t height ) const
	{
		return width <= m_context->texture_2d_dimension_max && height <= m_context->texture_2d_dimension_max;
	}
}
