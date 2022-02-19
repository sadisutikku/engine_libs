

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/platforms/clipboard/windows/clipboard_windows.h"

#include "core/platforms/include_wrappers/windows/windows.h"

namespace engine
{
	bool CLIPBOARD_WINDOWS::has_text( void* /*window_handle*/ )
	{
		return (bool)(::IsClipboardFormatAvailable( (uint32_t)CF_UNICODETEXT ) != 0);
	}

	bool CLIPBOARD_WINDOWS::get_text( std::string& text, void* window_handle )
	{
		if ( has_text( window_handle ) && ::OpenClipboard( nullptr ) )
		{
			HANDLE data = ::GetClipboardData( CF_UNICODETEXT );
			if ( data )
			{
				std::wstring ws( reinterpret_cast<wchar_t*>(data) );
				text = std::string( ws.begin(), ws.end() );
			}

			if ( const auto start_pos = text.find( "\r" ); start_pos != std::string::npos )
			{
				text.replace( start_pos, std::size( "\r" ), "" );
			}

			::CloseClipboard();
			return data != nullptr;
		}

		return false;
	}

	void CLIPBOARD_WINDOWS::set_text( const std::string& text, void* /*window_handle*/ )
	{
		if ( !text.empty() )
		{
			if ( ::OpenClipboard( nullptr ) )
			{
				if ( ::EmptyClipboard() )
				{
					std::string clipboard_text( text );
					if ( const auto start_pos = text.find( "\n" ); start_pos != std::string::npos )
					{
						clipboard_text.replace( start_pos, std::size( "\n" ), "\r\n" );
					}

					// allocate clipboard memory
					const auto length = clipboard_text.length();
					HGLOBAL block = ::GlobalAlloc( GMEM_DDESHARE, sizeof( char16_t ) * (length + 1) );

					if ( block )
					{
						// lock the memory and update
						HANDLE data = ::GlobalLock( block );
						if ( data )
						{
							std::wstring clipboard_text_w{ clipboard_text.begin(), clipboard_text.end() };
							memcpy( static_cast<char16_t*>(data), clipboard_text_w.c_str(), length );
							if ( ::SetClipboardData( CF_UNICODETEXT, data ) == nullptr )
							{
								// Clipboard failed and hasn't taken ownership, so we must free it
								::GlobalFree( block );
							}
							else
							{
								::GlobalUnlock( block );
							}
						}
					}
				}
				::CloseClipboard();
			}
		}
	}

	void CLIPBOARD_WINDOWS::set_image( const void* image_pixels, int32_t width, int32_t height )
	{
		// Check whether there are pixels available and valid width/hight are passed
		if ( image_pixels && width > 0 && height > 0 )
		{
			const DWORD img_width = (DWORD)width;
			const DWORD img_height = (DWORD)height;
			const DWORD size_pixels = img_width * img_height * 4;

			HGLOBAL hMem = ::GlobalAlloc( GHND, sizeof( BITMAPV5HEADER ) + size_pixels );
			if ( hMem )
			{
				BITMAPV5HEADER* hdr = (BITMAPV5HEADER*)::GlobalLock( hMem );
				if ( hdr )
				{
					hdr->bV5Size = sizeof( BITMAPV5HEADER );
					hdr->bV5Width = (LONG)img_width;
					hdr->bV5Height = -(LONG)img_height;
					hdr->bV5Planes = 1;
					hdr->bV5BitCount = 32;
					hdr->bV5Compression = BI_BITFIELDS;
					hdr->bV5SizeImage = size_pixels;
					hdr->bV5RedMask = 0x00ff0000;
					hdr->bV5GreenMask = 0x0000ff00;
					hdr->bV5BlueMask = 0x000000ff;
					hdr->bV5AlphaMask = 0xff000000;
					hdr->bV5CSType = LCS_WINDOWS_COLOR_SPACE;

					uint8_t* dst = (uint8_t*)hdr + sizeof( BITMAPV5HEADER );

					//sicore::MEMORY::copy( dst, image_pixels, size_pixels );

					if ( ::OpenClipboard( nullptr ) )
					{
						if ( ::EmptyClipboard() )
						{
							if ( SetClipboardData( CF_DIBV5, hMem ) == nullptr )
							{
								// Clipboard failed and hasn't taken ownership, so we must free it
								::GlobalFree( hMem );
							}
						}
						::CloseClipboard();
					}
				}
			}
		}
	}
}
