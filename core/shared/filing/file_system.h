
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <string>

namespace core
{
	class FILE_SYSTEM
	{
	public:
		static bool is_file( const std::string& path );

		static std::string get_filename_from_filepath( const std::string& filepath );

		static std::string get_directory_from_filepath( const std::string& filepath );
	};
}
