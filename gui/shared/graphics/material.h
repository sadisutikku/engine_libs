
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/vector2d.h"
#include "core/shared/math/vector4d.h"
#include "core/shared/misc/bitfields.h"

#include <type_traits>
#include <unordered_map>

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace gui
{
	class TEXTURE2D_BASE;
}

//////////////////////////////////////////////////////////////////////
//! Types
//////////////////////////////////////////////////////////////////////

namespace gui
{
	enum Material_Property : uint16_t
	{
		Material_Unknown = 0,
		Material_Clearcoat = 1 << 0,   // Extra white specular layer on top of others
		Material_Clearcoat_Roughness = 1 << 1,   // Roughness of clear-coat specular
		Material_Anisotropic = 1 << 2,   // Amount of anisotropy for specular reflection
		Material_Anisotropic_Rotation = 1 << 3,   // Rotates the direction of anisotropy, with 1.0 going full circle
		Material_Sheen = 1 << 4,   // Amount of soft velvet like reflection near edges
		Material_Sheen_Tint = 1 << 5,   // Mix between white and using base color for sheen reflection
		Material_Color = 1 << 6,   // Diffuse or metal surface color
		Material_Roughness = 1 << 7,   // Specifies micro-facet roughness of the surface for diffuse and specular reflection
		Material_Metallic = 1 << 8,   // Blends between a non-metallic and metallic material model
		Material_Normal = 1 << 9,   // Controls the normals of the base layers
		Material_Height = 1 << 10,  // Perceived depth for parallax mapping
		Material_Occlusion = 1 << 11,  // Amount of light loss, can be complementary to SSAO
		Material_Emission = 1 << 12,  // Light emission from the surface, works nice with bloom
		Material_Mask = 1 << 13   // Discards pixels
	};
}

namespace gui
{
	class MATERIAL
	{
	public:
		static MATERIAL* get_or_create_default_material();

		void set_texture_slot( Material_Property type, TEXTURE2D_BASE& texture );

		bool has_texture( const Material_Property texture_type ) const { return core::contains_bit( texture_type, m_flags ); }

		TEXTURE2D_BASE* get_texture_slot( const Material_Property type ) { return has_texture(type) ? m_textures.at(type) : nullptr; }

		const core::FVECTOR4D& get_color_albedo() const { return m_color_albedo; }

	private:
		std::unordered_map<Material_Property, TEXTURE2D_BASE*> m_textures;
		core::FVECTOR4D m_color_albedo{ 1.0f, 1.0f, 1.0f, 1.0f };
		core::FVECTOR2D m_uv_tiling{ 1.0f, 1.0f };
		core::FVECTOR2D m_uv_offset;
		uint16_t m_flags{};
		bool m_is_editable{ true };
	};
}
