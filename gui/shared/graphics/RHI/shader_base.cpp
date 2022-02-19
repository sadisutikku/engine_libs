

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/shader_base.h"

#include "core/shared/filing/file_system.h"

namespace gui
{
	void SHADER_BASE::compile( RENDERING_DEVICE_BASE& device, const RHI_Shader_Type shader_type, const core::RHI_Vertex_Type vertex_type, const std::string& shader )
	{
		m_shader_type = shader_type;
		m_vertex_type = vertex_type;

		if ( core::FILE_SYSTEM::is_file( shader ) )
		{
			m_name = core::FILE_SYSTEM::get_filename_from_filepath( shader );
			m_file_path = shader;
		}
		else
		{
			m_name.clear();
			m_file_path.clear();
		}

		// Compile
		m_compilation_state = Shader_Compilation_State::Compiling;
		m_resource = api_compile( device, shader );
		m_compilation_state = m_resource ? Shader_Compilation_State::Succeeded : Shader_Compilation_State::Failed;


	}

	std::string_view SHADER_BASE::GetEntryPoint() const
	{
		if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Vertex )     return "mainVS";
		if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Pixel )      return "mainPS";
		if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Compute )    return "mainCS";

		return {};
	}

	std::string_view SHADER_BASE::GetTargetProfile() const
	{
		if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Vertex )     return "vs_5_0";
		if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Pixel )      return "ps_5_0";
		if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Compute )    return "cs_5_0";

		return {};
	}

	std::string_view SHADER_BASE::GetShaderModel() const
	{
		return "5_0";
	}
}
