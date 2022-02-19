
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/imgui/imgui.h"

#include "core/shared/platform.h"
#include "core/shared/math/matrix.h"
#include "gui/shared/graphics/renderer/renderer.h"
#include "gui/shared/graphics/RHI/shader_base.h"
#include "gui/shared/window/window.h"

#if PLATFORM_WINDOWS
#include "gui/platforms/graphics/RHI/windows/d3d11/dxgi_info_manager.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/buffer_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_context_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/pipeline_state_d3d11.h"
#include "gui/platforms/graphics/renderer/windows/renderer_d3d11.h"
#endif

namespace
{
	std::unique_ptr<gui::BUFFER_D3D11> s_vertex_buffer;
	std::unique_ptr<gui::BUFFER_D3D11> s_index_buffer;
	gui::DXGI_INFO_MANAGER s_dxgi_info_manager;

	inline gui::SWAPCHAIN_BASE* get_swapchain( ImGuiViewport* viewport )
	{
		if ( !viewport )
		{
			ASSERT_FAILED( "Invalid viewport" );
			return nullptr;
		}

		auto swapchain = static_cast<gui::SWAPCHAIN_BASE*>(viewport->RendererUserData);

		if ( !swapchain )
		{
			ASSERT_FAILED( "Invalid swapchain" );
			return nullptr;
		}

		return swapchain;
	}
}

namespace gui::imgui
{
	void initialise( RENDERER& renderer, WINDOW& window )
	{
		IMGUI_CHECKVERSION();
		//ImGui::CreateContext();
		//ImGuiIO& io_ = ImGui::GetIO(); (void)io_;
		////io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		//// Setup Dear ImGui style
		//ImGui::StyleColorsDark();

		renderer.create_shader( RendererShader::IMGUI_VERTEX, RHI_Shader_Type::RHI_Shader_Vertex,
								core::RHI_Vertex_Type::RHI_Vertex_Type_Position2dTextureColor8, "imgui.hlsl" );
		renderer.create_shader( RendererShader::IMGUI_PIXEL, RHI_Shader_Type::RHI_Shader_Pixel, {}, "imgui.hlsl" );

		s_vertex_buffer = std::make_unique<BUFFER_D3D11>();
		s_index_buffer = std::make_unique<BUFFER_D3D11>();

		auto& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports | ImGuiBackendFlags_RendererHasVtxOffset;
		io.BackendRendererName = "renderer";
		if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
		{
			initialise_platform_interface();
		}

		if constexpr ( core::is_platform_windows() )
		{
			[[maybe_unused]] const auto result = ImGui_ImplWin32_Init( window.get_native_window() );
			ASSERT( result, "Call to ImGui_ImplWin32_Init failed" );

			auto renderer_d3d11 = static_cast<RENDERER_D3D11*>(RENDERER::get_singleton()->m_impl.get());
			ImGui_ImplDX11_Init( renderer_d3d11->get_device().get_device(), renderer_d3d11->get_device().get_context()->m_device_context.Get() );
			return;
		}

		// Font atlas
		{
			unsigned char* pixels;
			int atlas_width, atlas_height, bpp;
			auto& io = ImGui::GetIO();
			io.Fonts->GetTexDataAsRGBA32( &pixels, &atlas_width, &atlas_height, &bpp );

			// Copy pixel data
			const unsigned int size = atlas_width * atlas_height * bpp;
			std::vector<std::byte> data( size );
			data.reserve( size );
			memcpy( &data[0], reinterpret_cast<std::byte*>(pixels), size );

			// Upload texture to graphics system
			static TEXTURE2D_BASE* texture = renderer.create_texture2d( atlas_width, atlas_height, RHI_Format_R8G8B8A8_Unorm, RHI_Texture_Sampled, data );
			io.Fonts->TexID = static_cast<ImTextureID>(texture);
		}
	}

	void render( ImDrawData* draw_data, SWAPCHAIN_D3D11* swapchain )
	{
		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		const int fb_width = static_cast<int>(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
		const int fb_height = static_cast<int>(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
		if ( fb_width <= 0 || fb_height <= 0 || draw_data->TotalVtxCount == 0 )
		{
			return;
		}

		auto renderer_d3d11 = static_cast<RENDERER_D3D11*>(RENDERER::get_singleton()->m_impl.get());
		auto& device_d3d11 = renderer_d3d11->get_device();

		device_d3d11.get_context()->m_device_context->OMSetRenderTargets( 1u, swapchain->m_render_target_view.GetAddressOf(), nullptr );
		ImGui_ImplDX11_RenderDrawData( draw_data );
		return;

		if ( s_vertex_buffer->GetCount() < static_cast<uint32_t>(draw_data->TotalVtxCount) )
		{
			const uint32_t new_size = draw_data->TotalVtxCount + 5000;
			if ( !s_vertex_buffer->create_dynamic<ImDrawVert>( device_d3d11, BUFFER_TYPE::VERTEX, new_size ) )
			{
				return;
			}
		}
				
		if ( s_index_buffer->GetCount() < static_cast<uint32_t>(draw_data->TotalIdxCount) )
		{
			const uint32_t new_size = draw_data->TotalIdxCount + 10000;
			if ( !s_index_buffer->create_dynamic<ImDrawIdx>( device_d3d11, BUFFER_TYPE::INDEX, new_size ) )
			{
				return;
			}
		}

		// Copy and convert all vertices into a single contiguous buffer
		ImDrawVert* vtx_dst = static_cast<ImDrawVert*>(s_vertex_buffer->Map( device_d3d11 ));
		ImDrawIdx* idx_dst = static_cast<ImDrawIdx*>(s_index_buffer->Map( device_d3d11 ));
		if ( vtx_dst && idx_dst )
		{
			for ( auto i = 0; i < draw_data->CmdListsCount; i++ )
			{
				const ImDrawList* cmd_list = draw_data->CmdLists[i];
				memcpy( vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof( ImDrawVert ) );
				memcpy( idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof( ImDrawIdx ) );
				vtx_dst += cmd_list->VtxBuffer.Size;
				idx_dst += cmd_list->IdxBuffer.Size;
			}

			s_vertex_buffer->Unmap( device_d3d11 );
			s_index_buffer->Unmap( device_d3d11 );
		}

		static PIPELINE_STATE_D3D11 pipeline_state;
		pipeline_state.m_vertex_shader = renderer_d3d11->get_shader( RendererShader::IMGUI_VERTEX );
		pipeline_state.m_pixel_shader = renderer_d3d11->get_shader( RendererShader::IMGUI_PIXEL );
		//pipeline_state.rasterizer_state = g_rasterizer_state.get();
		//pipeline_state.blend_state = g_blend_state.get();
		//pipeline_state.depth_stencil_state = g_depth_stencil_state.get();
		//pipeline_state.vertex_buffer_stride = vertex_buffer->GetStride();
		pipeline_state.m_render_targets[0] = swapchain->Get_Resource_View_RenderTarget();
		//pipeline_state.clear_color[0] = clear ? Vector4( 0.0f, 0.0f, 0.0f, 1.0f ) : rhi_color_load;
		pipeline_state.m_viewport = { { 0.0f, 0.0f, draw_data->DisplaySize.x, draw_data->DisplaySize.y }, 0.0f, 1.0f };
		//pipeline_state.dynamic_scissor = true;
		pipeline_state.primitive_topology = RHI_PrimitiveTopology_TriangleList;
		//pipeline_state.pass_name = is_child_window ? "pass_imgui_window_child" : "pass_imgui_window_main";

		// Record commands
		if ( device_d3d11.get_context()->begin_render_pass( pipeline_state ) )
		{
			// Setup orthographic projection matrix into our constant buffer
			// Our visible ImGui space lies from draw_data->DisplayPos (top left) to 
			// draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is (0,0) for single viewport apps.
			{
				const float L = draw_data->DisplayPos.x;
				const float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
				const float T = draw_data->DisplayPos.y;
				const float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
				const core::MATRIX wvp = core::MATRIX
				(
					2.0f / (R - L), 0.0f, 0.0f, (R + L) / (L - R),
					0.0f, 2.0f / (T - B), 0.0f, (T + B) / (B - T),
					0.0f, 0.0f, 0.5f, 0.5f,
					0.0f, 0.0f, 0.0f, 1.0f
				);

				renderer_d3d11->set_global_shader_object_transform( wvp );
			}

			// Transition layouts
			for ( auto i = 0; i < draw_data->CmdListsCount; i++ )
			{
				auto cmd_list_imgui = draw_data->CmdLists[i];
				for ( int cmd_i = 0; cmd_i < cmd_list_imgui->CmdBuffer.Size; cmd_i++ )
				{
					const auto pcmd = &cmd_list_imgui->CmdBuffer[cmd_i];
					/*if ( RHI_Texture* texture = static_cast<RHI_Texture*>(pcmd->TextureId) )
					{
						texture->SetLayout( RHI_Image_Layout::Shader_Read_Only_Optimal, cmd_list );
					}*/
				}
			}

			device_d3d11.get_context()->set_vertex_buffer( *s_vertex_buffer, 0u );
			device_d3d11.get_context()->set_index_buffer( *s_index_buffer, 0u );

			int global_vtx_offset = 0;
			int global_idx_offset = 0;
			const ImVec2& clip_off = draw_data->DisplayPos;
			for ( int i = 0; i < draw_data->CmdListsCount; i++ )
			{
				ImDrawList* cmd_list_imgui = draw_data->CmdLists[i];
				for ( int cmd_i = 0; cmd_i < cmd_list_imgui->CmdBuffer.Size; cmd_i++ )
				{
					const ImDrawCmd* pcmd = &cmd_list_imgui->CmdBuffer[cmd_i];
					if ( pcmd->UserCallback != nullptr )
					{
						pcmd->UserCallback( cmd_list_imgui, pcmd );
					}
					else
					{
						core::FRECTANGLE scissor_rect{ pcmd->ClipRect.x - clip_off.x, pcmd->ClipRect.y - clip_off.y, 
											pcmd->ClipRect.z - clip_off.x, pcmd->ClipRect.w - clip_off.y };
						device_d3d11.get_context()->set_scissor_rect( scissor_rect );

						//device_d3d11.get_context()->set_texture( gui::TEXTURE_SLOTS::TEXTURE, static_cast<RHI_Texture*>(pcmd->TextureId) );

						s_dxgi_info_manager.Set();
						device_d3d11.get_context()->draw_indexed( pcmd->ElemCount, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset );
						for ( const auto& message : s_dxgi_info_manager.GetMessages() )
						{
							ASSERT_FAILED( message );
						}
					}

				}
				global_idx_offset += cmd_list_imgui->IdxBuffer.Size;
				global_vtx_offset += cmd_list_imgui->VtxBuffer.Size;
			}
		}
	}

	void platform_create_window( ImGuiViewport* viewport )
	{
		//::ImGui_ImplWin32_CreateWindow( viewport );
	}

	void RHI_Window_Create( ImGuiViewport* viewport )
	{
		if ( !viewport )
		{
			ASSERT_FAILED( "Invalid viewport" );
			return;
		}

		viewport->RendererUserData = RENDERER::get_singleton()->create_swapchain( reinterpret_cast<WINDOW_ID_TYPE>(viewport->PlatformHandle),
																				  viewport->Size.x, viewport->Size.y, RHI_Format_R8G8B8A8_Unorm, "" );
	}

	void RHI_Window_SetSize( ImGuiViewport* viewport, const ImVec2 size )
	{
		SWAPCHAIN_BASE* swap_chain = get_swapchain( viewport );
		if ( !swap_chain )
		{
			return;
		}

		if ( !swap_chain->resize( size.x, size.y ) )
		{
			ASSERT_FAILED( "Failed to resize swap chain" );
		}
	}

	void RHI_Window_Render( ImGuiViewport* viewport, void* )
	{
		SWAPCHAIN_BASE* swap_chain = get_swapchain( viewport );
		if ( !swap_chain )
		{
			return;
		}

		render( viewport->DrawData, static_cast<SWAPCHAIN_D3D11*>(swap_chain) );
	}

	void RHI_Window_Present( ImGuiViewport* viewport, void* )
	{
		SWAPCHAIN_BASE* swap_chain = get_swapchain( viewport );
		if ( !swap_chain )
		{
			return;
		}

		swap_chain->present();
	}

	void RHI_Window_Destroy( ImGuiViewport* viewport )
	{
		SWAPCHAIN_BASE* swap_chain = get_swapchain( viewport );
		if ( !swap_chain )
		{
			return;
		}

		delete swap_chain;
		viewport->RendererUserData = nullptr;
	}
}
