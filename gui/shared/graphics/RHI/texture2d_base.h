
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_types.h"
#include "gui/shared/graphics/RHI/viewport.h"

#include <array>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////
//! Types
//////////////////////////////////////////////////////////////////////

namespace gui
{
	enum class RENDER_TARGET : uint8_t
	{
		ALBEDO,
		NORMALS,
		VELOCITIES,

		REGION_TEST_0,
		REGION_TEST_1,
	};

	enum RHI_Texture_Flags : uint16_t
	{
		RHI_Texture_Sampled = 1 << 0,
		RHI_Texture_Storage = 1 << 1,
		RHI_Texture_RenderTarget = 1 << 2,
		RHI_Texture_DepthStencil = 1 << 3,
		RHI_Texture_DepthStencilReadOnly = 1 << 4,
		RHI_Texture_Grayscale = 1 << 5,
		RHI_Texture_Transparent = 1 << 6,
		RHI_Texture_GenerateMipsWhenLoading = 1 << 7
	};

	enum RHI_Shader_View_Type : uint8_t
	{
		RHI_Shader_View_ColorDepth,
		RHI_Shader_View_Stencil,
		RHI_Shader_View_Unordered_Access
	};
}

namespace gui
{
	class TEXTURE2D_BASE
	{
	public:
		TEXTURE2D_BASE( const uint32_t width, const uint32_t height, const RHI_Format format, const uint16_t flags, const std::vector<std::byte>& data )
			: m_width{ width }
			, m_height{ height }
			, m_format{ format }
			, m_viewport{ { 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height) }, 0.0f, 1.0f }
			, m_channel_count{ GetChannelCountFromFormat( format ) }
			, m_flags{ flags }
		{
			if ( !data.empty() )
			{
				m_data.emplace_back( data );
			}
		}

		static TEXTURE2D_BASE* load_from_file( const std::string& filepath );

		bool IsSampled()        const { return m_flags & RHI_Texture_Sampled; }
		bool IsStorage()        const { return m_flags & RHI_Texture_Storage; }
		bool IsDepthStencil()   const { return m_flags & RHI_Texture_DepthStencil; }
		bool IsRenderTarget()   const { return m_flags & RHI_Texture_RenderTarget; }

		// Format type
		bool IsDepthFormat()            const { return m_format == RHI_Format_D32_Float || m_format == RHI_Format_D32_Float_S8X24_Uint; }
		bool IsStencilFormat()          const { return m_format == RHI_Format_D32_Float_S8X24_Uint; }
		bool IsDepthStencilFormat()     const { return IsDepthFormat() || IsStencilFormat(); }
		bool IsColorFormat()            const { return !IsDepthStencilFormat(); }

		static uint32_t GetChannelCountFromFormat( const RHI_Format format )
		{
			switch ( format )
			{
			case RHI_Format_R8_Unorm:               return 1;
			case RHI_Format_R16_Uint:               return 1;
			case RHI_Format_R16_Float:              return 1;
			case RHI_Format_R32_Uint:               return 1;
			case RHI_Format_R32_Float:              return 1;
			case RHI_Format_R8G8_Unorm:             return 2;
			case RHI_Format_R16G16_Float:           return 2;
			case RHI_Format_R32G32_Float:           return 2;
			case RHI_Format_R11G11B10_Float:        return 3;
			case RHI_Format_R16G16B16A16_Snorm:     return 3;
			case RHI_Format_R32G32B32_Float:        return 3;
			case RHI_Format_R8G8B8A8_Unorm:         return 4;
			case RHI_Format_R10G10B10A2_Unorm:      return 4;
			case RHI_Format_R16G16B16A16_Float:     return 4;
			case RHI_Format_R32G32B32A32_Float:     return 4;
			case RHI_Format_D32_Float:              return 1;
			case RHI_Format_D32_Float_S8X24_Uint:   return 2;
			default:                                return 0;
			}
		}

		void* get_resource_view( const uint32_t i = 0 ) const { return m_resource_view[i]; }

		void* get_render_target_view( const uint32_t i = 0 ) const { return i < m_resource_view_renderTarget.size() ? m_resource_view_renderTarget[i] : nullptr; }

		RHI_Image_Layout GetLayout() const { return m_layout; }

		auto GetFormat() const { return m_format; }

		uint32_t get_width() const { return m_width; }
		uint32_t get_height() const { return m_height; }

	protected:
		uint8_t m_mip_count = 1;
		uint16_t m_flags = 0;
		RHI_Format m_format = RHI_Format_Undefined;
		RHI_Image_Layout m_layout = RHI_Image_Layout::Undefined;
		uint32_t m_bits_per_channel = 8;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_channel_count = 4;
		uint32_t m_array_size = 1;
		VIEWPORT m_viewport;
		std::vector< std::vector<std::byte> > m_data;

		void* m_resource_view[2] = { nullptr, nullptr }; // color/depth, stencil
		void* m_resource_view_unorderedAccess = nullptr;
		void* m_resource = nullptr;
		std::array<void*, rhi_max_render_target_count> m_resource_view_renderTarget = { nullptr };
		std::array<void*, rhi_max_render_target_count> m_resource_view_depthStencil = { nullptr };
		std::array<void*, rhi_max_render_target_count> m_resource_view_depthStencilReadOnly = { nullptr };
	};
}
