
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/material.h"

#include "gui/shared/graphics/storage/render_storage.h"

namespace gui
{
	MATERIAL* MATERIAL::get_or_create_default_material()
	{
		MATERIAL* material{ RENDER_STORAGE::get_singleton()->get_material( "default_material" ) };
		if ( !material )
		{
			material = RENDER_STORAGE::get_singleton()->create_material( "default_material" );
			auto colour_texture{ RENDER_STORAGE::get_singleton()->get_or_create_texture( "C:/dev/Silver/data/textures/sun.png" ) };
			material->set_texture_slot( Material_Property::Material_Color, *colour_texture );
		}

		return material;
	}

	void MATERIAL::set_texture_slot( const Material_Property type, TEXTURE2D_BASE& texture )
	{
		m_textures[type] = &texture;
		m_flags |= type;
	}
}
