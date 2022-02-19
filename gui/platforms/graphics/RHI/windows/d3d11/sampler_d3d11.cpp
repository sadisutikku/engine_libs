

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/sampler_d3d11.h"

#include "core/platforms/include_wrappers/windows/windows.h"
#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/d3d11_types.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"

namespace
{
	inline D3D11_FILTER get_filter( const gui::RHI_Filter filter_min, const gui::RHI_Filter filter_mag, const gui::RHI_Sampler_Mipmap_Mode filter_mipmap, bool anisotropy_enabled, bool comparison_enabled )
	{
		if ( anisotropy_enabled )
			return !comparison_enabled ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER_COMPARISON_ANISOTROPIC;

		if ( (filter_min == gui::RHI_Filter_Nearest) && (filter_mag == gui::RHI_Filter_Nearest) && (filter_mipmap == gui::RHI_Filter_Nearest) )            return !comparison_enabled ? D3D11_FILTER_MIN_MAG_MIP_POINT : D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		if ( (filter_min == gui::RHI_Filter_Nearest) && (filter_mag == gui::RHI_Filter_Nearest) && (filter_mipmap == gui::RHI_Sampler_Mipmap_Linear) )    return !comparison_enabled ? D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
		if ( (filter_min == gui::RHI_Filter_Nearest) && (filter_mag == gui::RHI_Sampler_Mipmap_Linear) && (filter_mipmap == gui::RHI_Filter_Nearest) )            return !comparison_enabled ? D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT : D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
		if ( (filter_min == gui::RHI_Filter_Nearest) && (filter_mag == gui::RHI_Sampler_Mipmap_Linear) && (filter_mipmap == gui::RHI_Sampler_Mipmap_Linear) )    return !comparison_enabled ? D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
		if ( (filter_min == gui::RHI_Sampler_Mipmap_Linear) && (filter_mag == gui::RHI_Filter_Nearest) && (filter_mipmap == gui::RHI_Filter_Nearest) )            return !comparison_enabled ? D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT : D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
		if ( (filter_min == gui::RHI_Sampler_Mipmap_Linear) && (filter_mag == gui::RHI_Filter_Nearest) && (filter_mipmap == gui::RHI_Sampler_Mipmap_Linear) )    return !comparison_enabled ? D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		if ( (filter_min == gui::RHI_Sampler_Mipmap_Linear) && (filter_mag == gui::RHI_Sampler_Mipmap_Linear) && (filter_mipmap == gui::RHI_Filter_Nearest) )            return !comparison_enabled ? D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT : D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		if ( (filter_min == gui::RHI_Sampler_Mipmap_Linear) && (filter_mag == gui::RHI_Sampler_Mipmap_Linear) && (filter_mipmap == gui::RHI_Sampler_Mipmap_Linear) )    return !comparison_enabled ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
}

namespace gui
{
	SAMPLER_D3D11::SAMPLER_D3D11(
		const RENDERING_DEVICE_D3D11& device,
		const RHI_Filter filter_min,
		const RHI_Filter filter_mag,
		const RHI_Sampler_Mipmap_Mode filter_mipmap,
		const RHI_Sampler_Address_Mode sampler_address_mode,
		const RHI_Comparison_Function comparison_function,
		const bool anisotropy_enabled,
		const bool comparison_enabled
	)
		: m_filter_min{ filter_min }
		, m_filter_mag{ filter_mag }
		, m_filter_mipmap{ filter_mipmap }
		, m_sampler_address_mode{ sampler_address_mode }
		, m_comparison_function{ comparison_function }
		, m_anisotropy_enabled{ anisotropy_enabled }
		, m_comparison_enabled{ comparison_enabled }
	{
		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.Filter = get_filter( m_filter_min, m_filter_mag, m_filter_mipmap, m_anisotropy_enabled, m_comparison_enabled );
		sampler_desc.AddressU = d3d11_sampler_address_mode[m_sampler_address_mode];
		sampler_desc.AddressV = d3d11_sampler_address_mode[m_sampler_address_mode];
		sampler_desc.AddressW = d3d11_sampler_address_mode[m_sampler_address_mode];
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 0u; // todo
		sampler_desc.ComparisonFunc = d3d11_comparison_function[m_comparison_function];
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = FLT_MAX;

		// Create sampler state.
		if ( FAILED( device.get_device()->CreateSamplerState( &sampler_desc, reinterpret_cast<ID3D11SamplerState**>(&m_resource) ) ) )
		{
			ASSERT_FAILED( "Failed to create sampler state" );
		}
	}

	SAMPLER_D3D11::~SAMPLER_D3D11() = default;
}
