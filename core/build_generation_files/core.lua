
PROJECT_NAME = "core"

project (PROJECT_NAME)
	location (project_directory(PROJECT_NAME, true))
	objdir (INTERMEDIATE_DIR)
	kind "StaticLib"
	staticruntime "On"
    conformancemode "On"
	
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
	
	--	"debug"
	filter "configurations:debug"
		targetdir (TARGET_DIR_DEBUG)
		debugdir (TARGET_DIR_DEBUG)
		debugformat (DEBUG_FORMAT)
			
	--	"release"
	filter "configurations:release"
		targetdir (TARGET_DIR_RELEASE)
		debugdir (TARGET_DIR_RELEASE)
