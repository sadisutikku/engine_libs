//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/importers/image_importer.h"

#include "gui/platforms/graphics/RHI/windows/d3d11/texture2d_d3d11.h"
#include "gui/shared/graphics/renderer/renderer.h"
#include "gui/shared/graphics/storage/render_storage.h"

#include <fstream>
#include <vector>

#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_STDIO
//#define STBI_MALLOC(sz)						sicore::MEMORY::allocate(sz)
//#define STBI_REALLOC(p,newsz)				sicore::MEMORY::reallocate(p,newsz)
//#define STBI_FREE(p)						sicore::MEMORY::free(p)
//#define STBI_REALLOC_SIZED(p,oldsz,newsz)	sicore::MEMORY::reallocate(p,newsz); SICORE_UNUSED(oldsz)
#include <gui/thirdparty/stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#define STBI_WRITE_NO_STDIO
//#define STBIW_MALLOC(sz)        sicore::MEMORY::allocate(sz)
//#define STBIW_REALLOC(p,newsz)  sicore::MEMORY::reallocate(p,newsz)
//#define STBIW_FREE(p)           sicore::MEMORY::free(p)
#include <gui/thirdparty/stb/stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_STATIC
// There is scope here to use the c (context) parameter to reuse temp buffers
//#define STBIR_MALLOC(sz,c) ((void)(c), sicore::MEMORY::allocate(sz))
//#define STBIR_FREE(p,c)    ((void)(c), sicore::MEMORY::free(p))
#include <gui/thirdparty/stb/stb_image_resize.h>

namespace
{
	struct CALLBACK_USER_DATA
	{
		std::vector<char>& data;
		uint32_t position{};
		bool is_eof{};
	};

	inline gui::RHI_Format get_rhi_format( const uint32_t bytes_per_channel, const uint32_t channel_count )
	{
		const uint32_t bits_per_channel = bytes_per_channel * 8;

		if ( channel_count == 1 )
		{
			if ( bits_per_channel == 8 )    return gui::RHI_Format_R8_Unorm;
		}
		else if ( channel_count == 2 )
		{
			if ( bits_per_channel == 8 )    return gui::RHI_Format_R8G8_Unorm;
		}
		else if ( channel_count == 3 )
		{
			if ( bits_per_channel == 32 ) return gui::RHI_Format_R32G32B32A32_Float;
		}
		else if ( channel_count == 4 )
		{
			if ( bits_per_channel == 8 )  return gui::RHI_Format_R8G8B8A8_Unorm;
			if ( bits_per_channel == 16 ) return gui::RHI_Format_R16G16B16A16_Float;
			if ( bits_per_channel == 32 ) return gui::RHI_Format_R32G32B32A32_Float;
		}

		ASSERT_FAILED( "Could not deduce format" );
		return gui::RHI_Format_Undefined;
	}
}

namespace gui
{
	int stream_read_callback( void *user, char *data, int size )
	{
		CALLBACK_USER_DATA* user_data = static_cast<CALLBACK_USER_DATA*>(user);
		memcpy( data, static_cast<const void*>(&user_data->data[user_data->position]), size );

		user_data->position += size;

		return size;
	}

	void stream_skip_callback( void *user, int size )
	{
		CALLBACK_USER_DATA* user_data = static_cast<CALLBACK_USER_DATA*>(user);
		
		user_data->position += size;
	}

	int stream_eof_callback( void *user )
	{
		const CALLBACK_USER_DATA* user_data = static_cast<CALLBACK_USER_DATA*>(user);
		return user_data->is_eof;
	}

	TEXTURE2D_BASE* IMAGE_IMPORTER::load_from_file( const std::string& filepath )
	{
		if ( auto existing_texture = RENDER_STORAGE::get_singleton()->get_texture( filepath ); existing_texture )
		{
			return existing_texture;
		}


		std::ifstream file_for_read( filepath, std::ios::binary );

		// Stop eating new lines in binary mode
		file_for_read.unsetf( std::ios::skipws );

		// get its size:
		std::streampos file_size;
		file_for_read.seekg( 0, std::ios::end );
		file_size = file_for_read.tellg();
		file_for_read.seekg( 0, std::ios::beg );

		std::vector<char> vec;
		vec.reserve( file_size );

		std::copy( std::istream_iterator<char>( file_for_read ),
				   std::istream_iterator<char>(),
				   std::back_inserter( vec ) );

		stbi_io_callbacks callbacks{ stream_read_callback, stream_skip_callback, stream_eof_callback };
		CALLBACK_USER_DATA user_data{ vec };

		int32_t width{};
		int32_t height{};
		int32_t channels{};
		
		if ( stbi_uc* ptr = stbi_load_from_callbacks( &callbacks, &user_data, &width, &height, &channels, STBI_rgb_alpha ) )
		{
			const int32_t num_bytes{ width * height * channels };
			auto byte_data{ reinterpret_cast<std::byte*>(ptr) };
			std::vector<std::byte> image_data{ byte_data, byte_data + num_bytes };

			const RHI_Format image_format = get_rhi_format( 1, channels );

			auto texture = RENDERER::get_singleton()->create_texture2d( width, height, image_format, RHI_Texture_Sampled, image_data );

			RENDER_STORAGE::get_singleton()->cache_texture( filepath, texture );

			return texture;
		}
		else
		{
			return nullptr;
		}
	}
}
