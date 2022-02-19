

function generate_library(in_library_name --[[str]])
	dofile (in_library_name)
end

function project_directory(in_project_name --[[str]], in_is_library --[[bool]])
	return path.join(in_is_library and LIB_DIRECTORY or ROOT_DIRECTORY, in_project_name, "projects", PLATFORM_NAME)
end

function create_project(in_project_name --[[str]], in_is_library --[[bool]])
	project (in_project_name)
		location (project_directory(in_project_name, in_is_library))
		objdir (INTERMEDIATE_DIR)
		staticruntime "On"
		conformancemode "On"
		
		if in_is_library == true then
			kind "StaticLib"
		else
			kind "WindowedApp"
		end
				
end

function create_static_library(in_library_name --[[str]])
	create_project(in_library_name, true)
end

function create_windowed_app(in_library_name --[[str]])
	create_project(in_library_name, false)
end


function get_thirdparty_directory(in_library_name --[[str]])
	return path.join(LIB_DIRECTORY, in_library_name, "thirdparty")
end
