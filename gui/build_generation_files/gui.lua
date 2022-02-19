
create_static_library("gui")
	
	includedirs 
    {
		LIB_DIRECTORY
	}
	
	-- Source
	files 
	{ 
		"../**.h",
		"../**.cpp",
		"../**.hpp",
		"../**.inl"
	}
	
	libdirs { TARGET_DIR_DEBUG }
	links
	{
		"core.lib",
		"engine.lib"
	}
	
	dependson
	{
		"engine"
	}

	libdirs { get_thirdparty_directory("gui") }
	links
	{
		"vulkan-1.lib",
		"spirv-cross-core_debug",
		"spirv-cross-glsl_debug",
		"spirv-cross-hlsl_debug",
		"dxcompiler",
	}

	--	"debug"
	filter "configurations:debug"
		targetdir (TARGET_DIR_DEBUG)
		debugdir (TARGET_DIR_DEBUG)
		debugformat (DEBUG_FORMAT)
			
	--	"release"
	filter "configurations:release"
		targetdir (TARGET_DIR_RELEASE)
		debugdir (TARGET_DIR_RELEASE)
