

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/filing/file_system.h"

#include <filesystem>

namespace core
{
	bool FILE_SYSTEM::is_file( const std::string& path )
	{
		if ( path.empty() )
		{
			return false;
		}

		try
		{
			if ( std::filesystem::exists( path ) && std::filesystem::is_regular_file( path ) )
			{
				return true;
			}
		}
		catch ( std::filesystem::filesystem_error& e )
		{
			//LOG_WARNING( "%s, %s", e.what(), path.c_str() );
		}

		return false;
	}

	std::string FILE_SYSTEM::get_filename_from_filepath( const std::string& filepath )
	{
		return std::filesystem::path( filepath ).filename().generic_string();
	}

	std::string FILE_SYSTEM::get_directory_from_filepath( const std::string& filepath )
	{
		const size_t last_index = filepath.find_last_of( "\\/" );

		if ( last_index != std::string::npos )
		{
			return filepath.substr( 0, last_index + 1 );
		}

		return "";
	}
}
