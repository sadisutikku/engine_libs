
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/renderer/windows/renderer_d3d11.h"

#include "core/shared/math/vector3d.h"
#include "engine/shared/application/application.h"
#include "engine/shared/game_framework/game_instance.h"
#include "engine/shared/game_framework/mesh.h"
#include "engine/shared/game_framework/world.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/buffer_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_context_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/dxgi_info_manager.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/pipeline_state_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/shader_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_swapchain_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/texture2d_d3d11.h"
#include "gui/shared/graphics/material.h"
#include "gui/shared/graphics/storage/render_storage.h"
#include "gui/shared/graphics/RHI/shader_bindings.h"
#include "gui/shared/graphics/RHI/RHI_types.h"
#include "gui/shared/screen/camera3d.h"
#include "gui/shared/screen/screen_manager.h"
#include "gui/shared/ui/widgets/widget.h"
#include "gui/shared/window/window.h"

#include <string>

namespace
{
	gui::DXGI_INFO_MANAGER s_dxgi_info_manager;
}

namespace gui
{
	RENDERER_D3D11::RENDERER_D3D11()
		: m_sampler_point_wrap{ m_device, RHI_Filter_Nearest, RHI_Filter_Nearest, RHI_Sampler_Mipmap_Nearest, RHI_Sampler_Address_Wrap, RHI_Comparison_Always }
		, m_rasterizer_cull_back_solid{ m_device, RHI_Cull_Back, RHI_Fill_Solid, true, false, false }
	{
		create_shaders();

		create_constant_buffers();

		create_render_targets();

		set_samplers();
	}

	RENDERER_D3D11::~RENDERER_D3D11() = default;

	void RENDERER_D3D11::setup_window( const WINDOW_ID_TYPE window_id )
	{
		const WINDOW* window = SCREEN_MANAGER::get_singleton()->get_window( window_id );
		const auto height = window->get_height();
		const auto width = window->get_width();
		HWND window_handle = window->get_native_window();

		/*core::VECTOR<SCREEN_AREA> areas;
		areas.emplace_back( SCREEN_AREA{ core::FRECTANGLE{ 0.0f, 0.0f, 800.0f, 600.0f }, new engine::WORLD, nullptr } );*/
		m_window_contexts.try_emplace( window_id, WINDOW_CONTEXT_D3D11{ {},
									   std::make_unique<SWAPCHAIN_D3D11>( window_handle, m_device, width, height, 
									   RHI_Format_R8G8B8A8_Unorm, RHI_Present_Immediate | RHI_Swap_Flip_Discard, "swapchain_main" ) } );
	}

	void RENDERER_D3D11::register_viewport( WIDGET* viewport_widget, CAMERA3D* camera3d, engine::WORLD* world )
	{
		auto [itr, success] = m_window_contexts.begin()->second.viewports.try_emplace( viewport_widget, VIEWPORT_DETAILS{ camera3d, world, nullptr } );

		switch ( VIEWPORT_DETAILS::s_num )
		{
		case 0:
			{
				m_render_targets[RENDER_TARGET::REGION_TEST_0] = std::make_unique<TEXTURE2D_D3D11>( *this, 800, 600, RHI_Format_R16G16B16A16_Float, RHI_Texture_Sampled | RHI_Texture_RenderTarget | RHI_Texture_Storage, std::vector<std::byte>{} );
				itr->second.render_target = m_render_targets[RENDER_TARGET::REGION_TEST_0].get();
			}
			break;
		case 1:
			{
				m_render_targets[RENDER_TARGET::REGION_TEST_1] = std::make_unique<TEXTURE2D_D3D11>( *this, 800, 600, RHI_Format_R16G16B16A16_Float, RHI_Texture_Sampled | RHI_Texture_RenderTarget | RHI_Texture_Storage, std::vector<std::byte>{} );
				itr->second.render_target = m_render_targets[RENDER_TARGET::REGION_TEST_1].get();
			}
			break;
		}

		++VIEWPORT_DETAILS::s_num;
	}

	void RENDERER_D3D11::create_shaders()
	{
		m_shaders[RendererShader::FIRST] = std::make_unique<SHADER_D3D11>();
		m_shaders[RendererShader::FIRST]->compile( m_device, RHI_Shader_Type::RHI_Shader_Vertex, core::RHI_Vertex_Type::RHI_Vertex_Type_Position, s_shader_dir + "first.hlsl" );
		
		m_shaders[RendererShader::FIRST_PIXEL] = std::make_unique<SHADER_D3D11>();
		m_shaders[RendererShader::FIRST_PIXEL]->compile( m_device, RHI_Shader_Type::RHI_Shader_Pixel, core::RHI_Vertex_Type::RHI_Vertex_Type_Unknown, s_shader_dir + "first.hlsl" );

		m_shaders[RendererShader::FIRST3D_VERTEX] = std::make_unique<SHADER_D3D11>();
		m_shaders[RendererShader::FIRST3D_VERTEX]->compile( m_device, RHI_Shader_Type::RHI_Shader_Vertex, core::RHI_Vertex_Type::RHI_Vertex_Type_PositionTextureNormalTangent, s_shader_dir + "first3d.hlsl" );

		m_shaders[RendererShader::FIRST3D_PIXEL] = std::make_unique<SHADER_D3D11>();
		m_shaders[RendererShader::FIRST3D_PIXEL]->compile( m_device, RHI_Shader_Type::RHI_Shader_Pixel, core::RHI_Vertex_Type::RHI_Vertex_Type_Unknown, s_shader_dir + "first3d.hlsl" );

		m_shaders[RendererShader::QUAD_VERTEX] = std::make_unique<SHADER_D3D11>();
		m_shaders[RendererShader::QUAD_VERTEX]->compile( m_device, RHI_Shader_Type::RHI_Shader_Vertex, core::RHI_Vertex_Type::RHI_Vertex_Type_PositionTexture, s_shader_dir + "quad.hlsl" );

		m_shaders[RendererShader::TEXTURE_PIXEL] = std::make_unique<SHADER_D3D11>();
		m_shaders[RendererShader::TEXTURE_PIXEL]->compile( m_device, RHI_Shader_Type::RHI_Shader_Pixel, core::RHI_Vertex_Type::RHI_Vertex_Type_Unknown, s_shader_dir + "quad.hlsl" );
	}

	void RENDERER_D3D11::create_constant_buffers()
	{
		m_global_constant_buffer.create_dynamic<GLOBAL_CONSTANT_BUFFER>( m_device, BUFFER_TYPE::CONSTANT, sizeof( GLOBAL_CONSTANT_BUFFER ) );
		m_frame_constant_buffer.create_dynamic<FRAME_CONSTANT_BUFFER>( m_device, BUFFER_TYPE::CONSTANT, sizeof( FRAME_CONSTANT_BUFFER ) );
	}

	void RENDERER_D3D11::create_render_targets()
	{
		m_render_targets[RENDER_TARGET::REGION_TEST_0] = std::make_unique<TEXTURE2D_D3D11>( *this, 800, 600, RHI_Format_R16G16B16A16_Float, RHI_Texture_Sampled | RHI_Texture_RenderTarget | RHI_Texture_Storage, std::vector<std::byte>{} );
	}

	void RENDERER_D3D11::set_samplers()
	{
		m_device.get_context()->set_sampler( SAMPLER_SLOTS::POINT_CLAMP, &m_sampler_point_wrap );
	}

	void RENDERER_D3D11::create_shader( const RendererShader shader_id, const RHI_Shader_Type shader_type, const core::RHI_Vertex_Type vertex_type,
										const std::string& shader_name )
	{
		m_shaders[shader_id] = std::make_unique<SHADER_D3D11>();
		m_shaders[shader_id]->compile( m_device, shader_type, vertex_type, s_shader_dir + shader_name );
	}

	SHADER_D3D11* RENDERER_D3D11::get_shader( const RendererShader shader_id ) const
	{
		if ( auto itr = m_shaders.find( shader_id ); itr != m_shaders.end() )
		{
			return itr->second.get();
		}
		else
		{
			return nullptr;
		}
	}

	TEXTURE2D_BASE* RENDERER_D3D11::create_texture2d( const uint32_t width, const uint32_t height, const RHI_Format format, const uint16_t flags, const std::vector<std::byte>& data )
	{
		return new TEXTURE2D_D3D11{ *this, width, height, format, flags, data };
	}

	SWAPCHAIN_BASE* RENDERER_D3D11::create_swapchain( const WINDOW_ID_TYPE window_id, const float width, const float height, const RHI_Format format, const std::string& name )
	{
		return new SWAPCHAIN_D3D11( reinterpret_cast<HWND>(window_id), m_device, width, height, format, m_window_contexts.begin()->second.swapchain->get_flags(), name.c_str() );
	}

	TEXTURE2D_D3D11* RENDERER_D3D11::get_viewport( WIDGET* viewport_widget ) const
	{
		return m_window_contexts.begin()->second.viewports.at( viewport_widget ).render_target;
	}

	void RENDERER_D3D11::on_mesh_created( engine::MESH* mesh )
	{
		auto vertex_buffer = create_buffer<BUFFER_D3D11, core::RHI_Vertex_PosTexNorTan>( m_device, BUFFER_TYPE::VERTEX, mesh->get_vertices() );
		auto index_buffer = create_buffer<BUFFER_D3D11, uint32_t>( m_device, BUFFER_TYPE::INDEX, mesh->get_indices() );
		m_device.get_context()->set_vertex_buffer( *vertex_buffer, 0u );
		m_device.get_context()->set_index_buffer( *index_buffer, 0u );
	}

	void RENDERER_D3D11::update_global_constant_buffer()
	{
		void* mapped_data = m_global_constant_buffer.Map( m_device );
		if ( mapped_data )
		{
			memcpy( reinterpret_cast<std::byte*>(mapped_data), reinterpret_cast<std::byte*>(&m_global_constant_buffer_cpu), m_global_constant_buffer.GetStride() );
		
			m_global_constant_buffer.Unmap( m_device );

			constexpr auto buffer_register_slot{ 2u };
			constexpr uint8_t shader_types{ (uint8_t)RHI_Shader_Type::RHI_Shader_Vertex | (uint8_t)RHI_Shader_Type::RHI_Shader_Pixel | (uint8_t)RHI_Shader_Type::RHI_Shader_Compute };

			// Dynamic buffers with offsets have to be rebound whenever the offset changes
			m_device.get_context()->set_constant_buffer( buffer_register_slot, shader_types, m_global_constant_buffer );
		}
	}

	void RENDERER_D3D11::update_frame_constant_buffer()
	{
		void* mapped_data = m_frame_constant_buffer.Map( m_device );
		if ( mapped_data )
		{
			memcpy( reinterpret_cast<std::byte*>(mapped_data), reinterpret_cast<std::byte*>(&m_frame_constant_buffer_cpu), m_frame_constant_buffer.GetStride() );

			m_frame_constant_buffer.Unmap( m_device );

			constexpr auto buffer_register_slot{ 0u };
			constexpr uint8_t shader_types{ (uint8_t)RHI_Shader_Type::RHI_Shader_Vertex | (uint8_t)RHI_Shader_Type::RHI_Shader_Pixel | (uint8_t)RHI_Shader_Type::RHI_Shader_Compute };

			// Dynamic buffers with offsets have to be rebound whenever the offset changes
			m_device.get_context()->set_constant_buffer( buffer_register_slot, shader_types, m_frame_constant_buffer );
		}
	}

	void RENDERER_D3D11::pass_main()
	{
		const WINDOW_ID_TYPE window_id{ m_window_contexts.begin()->first }; // temp

		auto itr = m_window_contexts.find( window_id );
		if ( itr == m_window_contexts.end() )
		{
			return;
		}

		for ( auto& [key, window_context] : m_window_contexts )
		{
			window_context.swapchain->clear_backbuffer( { 0.0f, 0.0f, 0.0f } );

			for ( auto& [key, viewport] : window_context.viewports )
			{
				m_device.get_context()->clear_render_target_view( static_cast<ID3D11RenderTargetView*>(viewport.render_target->get_render_target_view()), { 0.0f, 0.0f, 0.0f } );
			}
		}

		//pass_update_framebuffer( window_id );

		pass_gbuffer( window_id );

		//pass_copy_to_framebuffer( window_id );

		//itr->second.swapchain->present();
	}

	void RENDERER_D3D11::pass_update_framebuffer( const WINDOW_ID_TYPE window_id )
	{
		PIPELINE_STATE_D3D11 pipeline_state;

		if ( m_device.get_context()->begin_render_pass( pipeline_state ) )
		{
			update_framebuffer();
		}
	}

	bool RENDERER_D3D11::update_framebuffer()
	{


		return true;
	}

	void RENDERER_D3D11::pass_gbuffer( const WINDOW_ID_TYPE window_id )
	{
		auto itr = m_window_contexts.find( window_id );
		if ( itr == m_window_contexts.end() )
		{
			return;
		}

		const WINDOW_CONTEXT_D3D11& window_context{ itr->second };

		//for ( auto& screen_area : window_context.screen_areas )
		for ( auto& [map_key, viewport] : window_context.viewports )
		{
			if ( viewport.world->get_world_objects().size() == 0 )
			{
				continue;
			}

			auto vertex_shader = m_shaders[RendererShader::FIRST3D_VERTEX].get();
			auto pixel_shader = m_shaders[RendererShader::FIRST3D_PIXEL].get();

			PIPELINE_STATE_D3D11 pipeline_state;
			pipeline_state.m_vertex_shader = vertex_shader;
			pipeline_state.m_pixel_shader = pixel_shader;
			pipeline_state.m_render_targets[0] = static_cast<ID3D11RenderTargetView*>(viewport.render_target->get_render_target_view());//m_render_targets[RENDER_TARGET::REGION_TEST_0]->get_render_target_view());
			pipeline_state.m_rasteriser_state = &m_rasterizer_cull_back_solid;
			pipeline_state.primitive_topology = RHI_PrimitiveTopology_TriangleList;
			pipeline_state.m_viewport = { { 0.0f, 0.0f, 800.0f, 600.0f }, 0.0f, 1.0f };

			if ( m_device.get_context()->begin_render_pass( pipeline_state ) )
			{
				auto view_matrix{ viewport.camera3d->get_view_matrix() };
				auto projection_matrix{ viewport.camera3d->get_projection_matrix() };

				m_frame_constant_buffer_cpu.view = view_matrix;// core::MATRIX::Identity;
				m_frame_constant_buffer_cpu.projection = projection_matrix;// core::MATRIX::Identity;
				m_frame_constant_buffer_cpu.view_projection = m_frame_constant_buffer_cpu.view * m_frame_constant_buffer_cpu.projection;
				update_frame_constant_buffer();

				for ( auto world_object : viewport.world->get_world_objects() )
				{
					auto& transform{ world_object->get_transform() };

					// @todo Compute new tranform taking special relativity into account
					// atm assuming that camera was stationary this frame

					transform.compute_matrix(); // @todo optimise to only call this if we know the tranform is dirty
					const auto transform_matrix{ transform.get_matrix() };

					set_global_shader_object_transform( transform_matrix /*core::MATRIX::Identity*/ );

					if ( auto material{ world_object->get_mesh()->m_material }; material )
					{
						if ( auto colour_tex{ material->get_texture_slot( Material_Property::Material_Color ) }; colour_tex )
						{
							m_device.get_context()->set_texture( TEXTURE_SLOTS::MATERIAL_ALBEDO, static_cast<TEXTURE2D_D3D11*>(colour_tex) );
						}
					}

					const auto num_indices = world_object->get_mesh()->get_indices().size();

					s_dxgi_info_manager.Set();
					m_device.get_context()->draw_indexed( static_cast<uint32_t>(num_indices), 0u, 0u );
					for ( const auto& message : s_dxgi_info_manager.GetMessages() )
					{
						ASSERT_FAILED( message );
					}
				}
			}
		}
	}

	void RENDERER_D3D11::pass_copy_to_framebuffer( const WINDOW_ID_TYPE window_id )
	{
		auto itr = m_window_contexts.find( window_id );
		if ( itr == m_window_contexts.end() )
		{
			return;
		}

		const WINDOW_CONTEXT_D3D11& window_context{ itr->second };

		auto vertex_shader = m_shaders[RendererShader::QUAD_VERTEX].get();
		ASSERT( vertex_shader, "Vertex Shader is nullptr" );

		auto pixel_shader = m_shaders[RendererShader::TEXTURE_PIXEL].get();
		ASSERT( pixel_shader, "Pixel Shader is nullptr" );

		PIPELINE_STATE_D3D11 pipeline_state;
		pipeline_state.m_vertex_shader = vertex_shader;
		pipeline_state.m_pixel_shader = pixel_shader;
		pipeline_state.m_render_targets[0] = window_context.swapchain->Get_Resource_View_RenderTarget();
		pipeline_state.primitive_topology = RHI_PrimitiveTopology_TriangleList;
		pipeline_state.m_viewport = { { 0.0f, 0.0f, 800.0f, 600.0f }, 0.0f, 1.0f };

		if ( m_device.get_context()->begin_render_pass( pipeline_state ) )
		{
			const auto& swapchain = window_context.swapchain;

			//swapchain->clear_backbuffer( { 0.0f, 0.0f, 1.0f } );

			m_global_constant_buffer_cpu.resolution = swapchain->get_dimensions();
			update_global_constant_buffer();

			const auto near_plane{ 0.3f };
			const auto far_plane{ 1000.0f };
			m_frame_constant_buffer_cpu.projection_ortho = core::MATRIX::CreateOrthographicLH( pipeline_state.m_viewport.m_area.get_width(), pipeline_state.m_viewport.m_area.get_height(), near_plane, far_plane );
			m_frame_constant_buffer_cpu.view_projection_ortho = core::MATRIX::CreateLookAtLH( { 0.0f, 0.0f, -near_plane }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }/*core::FORWARD_VECTOR, core::UP_VECTOR*/ ) * m_frame_constant_buffer_cpu.projection_ortho;
			update_frame_constant_buffer();

			m_device.get_context()->set_texture( TEXTURE_SLOTS::TEXTURE, m_render_targets[RENDER_TARGET::REGION_TEST_0].get() );
			m_device.get_context()->set_vertex_buffer( *swapchain->get_viewport_quad().get_vertex_buffer(), 0u );
			m_device.get_context()->set_index_buffer( *swapchain->get_viewport_quad().get_index_buffer(), 0u );

			s_dxgi_info_manager.Set();
			m_device.get_context()->draw_indexed( QUAD_D3D11::get_num_indices(), 0u, 0u );
			for ( const auto& message : s_dxgi_info_manager.GetMessages() )
			{
				ASSERT_FAILED( message );
			}

			//swapchain->present();
		}
	}

	void RENDERER_D3D11::present()
	{
		for ( auto& [key, window_context] : m_window_contexts )
		{
			window_context.swapchain->present();
		}
	}
}
