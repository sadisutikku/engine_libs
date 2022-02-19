
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_types.h"

//////////////////////////////////////////////////////////////////////
//! Constants
//////////////////////////////////////////////////////////////////////

namespace gui
{
	/*inline constexpr RHI_Filter SAMPLER_POINT{ RHI_Filter_Nearest | RHI_Filter_Nearest | RHI_Sampler_Mipmap_Nearest };
	inline constexpr RHI_Filter SAMPLER_BILINEAR{ RHI_Filter_Linear | RHI_Filter_Linear | RHI_Sampler_Mipmap_Nearest };
	inline constexpr RHI_Filter SAMPLER_TRILINEAR{ RHI_Filter_Linear | RHI_Filter_Linear | RHI_Sampler_Mipmap_Linear };*/
}

namespace gui
{
	class RENDERER_D3D11;
	class RENDERING_DEVICE_D3D11;

	class SAMPLER_D3D11
	{
	public:
		SAMPLER_D3D11(
			const RENDERING_DEVICE_D3D11& device,
			const RHI_Filter filter_min = RHI_Filter_Nearest,
			const RHI_Filter filter_mag = RHI_Filter_Nearest,
			const RHI_Sampler_Mipmap_Mode filter_mipmap = RHI_Sampler_Mipmap_Nearest,
			const RHI_Sampler_Address_Mode sampler_address_mode = RHI_Sampler_Address_Wrap,
			const RHI_Comparison_Function comparison_function = RHI_Comparison_Always,
			const bool anisotropy_enabled = false,
			const bool comparison_enabled = false
		);

		~SAMPLER_D3D11();

		RHI_Filter GetFilterMin()                       const { return m_filter_min; }
		RHI_Filter GetFilterMag()                       const { return m_filter_mag; }
		RHI_Sampler_Mipmap_Mode GetFilterMipmap()       const { return m_filter_mipmap; }
		RHI_Sampler_Address_Mode GetAddressMode()       const { return m_sampler_address_mode; }
		RHI_Comparison_Function GetComparisonFunction() const { return m_comparison_function; }
		bool GetAnisotropyEnabled()                     const { return m_anisotropy_enabled; }
		bool GetComparisonEnabled()                     const { return m_comparison_enabled; }
		void* GetResource()                             const { return m_resource; }

	private:
		RHI_Filter m_filter_min = RHI_Filter_Nearest;
		RHI_Filter m_filter_mag = RHI_Filter_Nearest;
		RHI_Sampler_Mipmap_Mode m_filter_mipmap = RHI_Sampler_Mipmap_Nearest;
		RHI_Sampler_Address_Mode m_sampler_address_mode = RHI_Sampler_Address_Wrap;
		RHI_Comparison_Function m_comparison_function = RHI_Comparison_Always;
		bool m_anisotropy_enabled = false;
		bool m_comparison_enabled = false;

		// API
		void* m_resource = nullptr;
	};
}
