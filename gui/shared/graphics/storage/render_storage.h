
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/non_automatic_singleton.h"

#include "core/shared/type_wrappers/map.h"
#include "gui/shared/graphics/material.h"
#include "gui/shared/graphics/RHI/texture2d_base.h"

#include <memory>
#include <string>

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace gui
{
	class MATERIAL;
	class TEXTURE2D_BASE;
}

namespace gui
{
	class RENDER_STORAGE : public core::NON_AUTOMATIC_SINGLETON<RENDER_STORAGE>
	{
	public:
		template<typename TYPE>
		TYPE* load_resource( const std::string& resource_path )
		{
			if ( !file_exists( resource_path ) )
			{
				return nullptr;
			}

			TYPE* object_of_type = TYPE::load_from_file( resource_path );

			return object_of_type;
		}

		bool has_material( const std::string& material_path );

		MATERIAL* get_material( const std::string& material_path );

		MATERIAL* get_or_create_material( const std::string& material_path );

		MATERIAL* create_material( const std::string& material_path );

		void cache_material( const std::string& material_path, MATERIAL* material );

		bool has_texture( const std::string& texture_path );

		TEXTURE2D_BASE* get_texture( const std::string& texture_path );

		TEXTURE2D_BASE* get_or_create_texture( const std::string& texture_path );

		TEXTURE2D_BASE* create_texture( const std::string& texture_path );

		void cache_texture( const std::string& texture_path, TEXTURE2D_BASE* texture );

	private:
		bool file_exists( const std::string& resource_path ) const;

		core::MAP< std::string_view, std::unique_ptr<MATERIAL> > m_materials;
		core::MAP< std::string_view, std::unique_ptr<TEXTURE2D_BASE> > m_textures;
	};
}
