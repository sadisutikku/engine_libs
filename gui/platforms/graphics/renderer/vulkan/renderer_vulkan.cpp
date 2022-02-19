
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/renderer/vulkan/renderer_vulkan.h"

#include "core/shared/math/vector3d.h"
#include "engine/shared/application/application.h"

#include "engine/shared/game_framework/game_instance.h"
#include "engine/shared/game_framework/mesh.h"
#include "engine/shared/game_framework/world.h"
#include "gui/shared/graphics/material.h"
#include "gui/shared/graphics/storage/render_storage.h"
#include "gui/shared/graphics/RHI/shader_bindings.h"
#include "gui/shared/screen/camera3d.h"
#include "gui/shared/screen/screen_manager.h"
#include "gui/shared/ui/widgets/widget.h"

#include "gui/platforms/graphics/RHI/vulkan/descriptor_set_layout_cache_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/descriptor_set_layout_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/pipeline_state_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/pipeline_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/shader_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/swapchain_vulkan.h"
#include "gui/shared/graphics/RHI/RHI_types.h"
#include "gui/shared/window/window.h"

#include <string>

namespace gui
{
	RENDERER_VULKAN::RENDERER_VULKAN()
	{
		create_shaders();
	}

	RENDERER_VULKAN::~RENDERER_VULKAN() = default;

	void RENDERER_VULKAN::setup_window( const WINDOW_ID_TYPE window_id )
	{
		const WINDOW* window = SCREEN_MANAGER::get_singleton()->get_window( window_id );
		const auto height = window->get_height();
		const auto width = window->get_width();
		HWND window_handle = window->get_native_window();

		/*core::VECTOR<SCREEN_AREA> areas;
		areas.emplace_back( SCREEN_AREA{ core::FRECTANGLE{ 0.0f, 0.0f, 800.0f, 600.0f }, new engine::WORLD, nullptr } );*/
		m_window_contexts.try_emplace( window_id, WINDOW_CONTEXT_VULKAN{ {},
									   std::make_unique<SWAPCHAIN_VULKAN>( window_handle, m_device, width, height,
									   RHI_Format_R8G8B8A8_Unorm, RHI_Present_Immediate | RHI_Swap_Flip_Discard, "swapchain_main" ) } );
	}

	void RENDERER_VULKAN::create_shaders()
	{
		m_shaders[RendererShader::FIRST] = std::make_unique<SHADER_VULKAN>();
		m_shaders[RendererShader::FIRST]->compile( m_device, RHI_Shader_Type::RHI_Shader_Vertex, core::RHI_Vertex_Type::RHI_Vertex_Type_Position, s_shader_dir + "first.hlsl" );
	
		m_shaders[RendererShader::FIRST_PIXEL] = std::make_unique<SHADER_VULKAN>();
		m_shaders[RendererShader::FIRST_PIXEL]->compile( m_device, RHI_Shader_Type::RHI_Shader_Pixel, core::RHI_Vertex_Type::RHI_Vertex_Type_Unknown, s_shader_dir + "first.hlsl" );
	}

	void RENDERER_VULKAN::pass_main()
	{
		pass_gbuffer( {} );
	}

	void RENDERER_VULKAN::pass_gbuffer( const WINDOW_ID_TYPE /*window_id*/ )
	{
		m_descriptor_set_layout_cache = std::make_unique<DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN>( m_device );

		PIPELINE_STATE_VULKAN pipeline_state{ m_device };
		pipeline_state.m_vertex_shader = m_shaders.at( RendererShader::FIRST ).get();
		pipeline_state.m_pixel_shader = m_shaders.at( RendererShader::FIRST_PIXEL ).get();

		m_descriptor_set_layout_cache->SetPipelineState( pipeline_state );

		m_pipeline = std::make_unique<PIPELINE_VULKAN>( m_device, pipeline_state, m_descriptor_set_layout_cache->GetCurrentDescriptorSetLayout() );


	}

	void RENDERER_VULKAN::present()
	{
		
	}
}
