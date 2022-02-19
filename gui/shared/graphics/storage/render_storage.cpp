

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/storage/render_storage.h"

#include "core/shared/filing/file_system.h"
#include "gui/shared/graphics/material.h"
#include "gui/shared/graphics/RHI/texture2d_base.h"

namespace gui
{
	bool RENDER_STORAGE::has_material( const std::string& material_path )
	{
		return get_material( material_path ) != nullptr;
	}

	MATERIAL* RENDER_STORAGE::get_material( const std::string& material_path )
	{
		auto itr = m_materials.find( material_path );
		return itr != m_materials.end() ? itr->second.get() : nullptr;
	}

	MATERIAL* RENDER_STORAGE::get_or_create_material( const std::string& material_path )
	{
		auto material{ get_material( material_path ) };
		return material ? material : create_material( material_path );
	}

	MATERIAL* RENDER_STORAGE::create_material( const std::string& material_path )
	{
		m_materials[material_path] = std::make_unique<MATERIAL>();
		return m_materials.at( material_path ).get();
	}

	void RENDER_STORAGE::cache_material( const std::string& material_path, MATERIAL* material )
	{
		m_materials[material_path] = std::unique_ptr<MATERIAL>( material );
	}

	bool RENDER_STORAGE::has_texture( const std::string& texture_path )
	{
		return get_material( texture_path ) != nullptr;
	}

	TEXTURE2D_BASE* RENDER_STORAGE::get_texture( const std::string& texture_path )
	{
		auto itr = m_textures.find( texture_path );
		return itr != m_textures.end() ? itr->second.get() : nullptr;
	}

	TEXTURE2D_BASE* RENDER_STORAGE::get_or_create_texture( const std::string& texture_path )
	{
		auto texture{ get_texture( texture_path ) };
		return texture ? texture : create_texture( texture_path );
	}

	TEXTURE2D_BASE* RENDER_STORAGE::create_texture( const std::string& texture_path )
	{
		return load_resource<TEXTURE2D_BASE>( texture_path );
	}

	void RENDER_STORAGE::cache_texture( const std::string& texture_path, TEXTURE2D_BASE* texture )
	{
		m_textures[texture_path] = std::unique_ptr<TEXTURE2D_BASE>( texture );
	}

	bool RENDER_STORAGE::file_exists( const std::string& resource_path ) const
	{
		return core::FILE_SYSTEM::is_file( resource_path );
	}
}
