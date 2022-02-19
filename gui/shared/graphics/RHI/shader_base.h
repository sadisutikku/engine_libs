
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/vertex.h"
#include "gui/shared/graphics/RHI/descriptor.h"
#include "gui/shared/graphics/RHI/RHI_types.h"

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace gui
{
	class INPUT_LAYOUT;
	class RENDERING_DEVICE_BASE;

	enum class RendererShader : uint8_t
	{
		FIRST,
		FIRST_PIXEL,
		FIRST3D_VERTEX,
		FIRST3D_PIXEL,
		IMGUI_VERTEX,
		IMGUI_PIXEL,
		QUAD_VERTEX,
		TEXTURE_PIXEL,
	};

	inline const std::string s_shader_dir{ "C:/dev/Silver/data/shaders/" };

	class SHADER_BASE
	{
	public:
		void compile( RENDERING_DEVICE_BASE& device, RHI_Shader_Type shader_type, core::RHI_Vertex_Type vertex_type, const std::string& shader );

		INPUT_LAYOUT* get_input_layout() const { return m_input_layout.get(); }

		void* GetResource() const { return m_resource; }

		std::string_view GetEntryPoint() const;

		const std::vector<DESCRIPTOR>& get_descriptors() const { return m_descriptors; }
		std::vector<DESCRIPTOR>& get_descriptors() { return m_descriptors; }

	protected:
		virtual void* api_compile( RENDERING_DEVICE_BASE& device, const std::string& shader ) = 0;

		std::string_view GetTargetProfile() const;

		std::string_view GetShaderModel() const;

		std::string m_name;
		std::string m_file_path;
		std::unordered_map<std::string, std::string> m_defines;
		std::vector<DESCRIPTOR> m_descriptors;
		std::unique_ptr<INPUT_LAYOUT> m_input_layout;
		std::atomic<Shader_Compilation_State> m_compilation_state = Shader_Compilation_State::Idle;
		RHI_Shader_Type m_shader_type = RHI_Shader_Type::RHI_Shader_Unknown;
		core::RHI_Vertex_Type m_vertex_type{ core::RHI_Vertex_Type::RHI_Vertex_Type_Unknown };
		void* m_resource{ nullptr };
	};
}
