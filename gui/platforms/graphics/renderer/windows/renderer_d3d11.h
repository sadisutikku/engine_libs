
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/renderer/renderer_base.h"

#include "gui/platforms/graphics/RHI/windows/d3d11/command_list_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/buffer_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/sampler_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/rasteriser_state_d3d11.h"
#include "gui/shared/screen/camera3d.h"
#include "gui/shared/window/window_details.h"

#include <memory>
#include <unordered_map>
#include <vector>

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace core
{
	enum class RHI_Vertex_Type : uint8_t;
}

namespace engine
{
	class WORLD;
}

namespace gui
{
	class SHADER_D3D11;
	class TEXTURE2D_BASE;
	class TEXTURE2D_D3D11;
	class WIDGET;
	enum class RendererShader : uint8_t;
	enum class RENDER_TARGET : uint8_t;
	enum class RHI_Shader_Type : uint8_t;

	struct VIEWPORT_DETAILS
	{
		CAMERA3D* camera3d{};
		engine::WORLD* world{};
		TEXTURE2D_D3D11* render_target{};

		inline static uint8_t s_num{ 0u };
	};

	struct WINDOW_CONTEXT_D3D11 : public WINDOW_CONTEXT
	{
		std::unique_ptr<SWAPCHAIN_D3D11> swapchain;
		std::unordered_map<void*, VIEWPORT_DETAILS> viewports;
	};

	class RENDERER_D3D11 : public RENDERER_BASE
	{
	public:
		RENDERER_D3D11();
		~RENDERER_D3D11();

		void setup_window( WINDOW_ID_TYPE window_id );

		void register_viewport( WIDGET* viewport_widget, CAMERA3D* camera3d, engine::WORLD* world );

		void create_shader( RendererShader shader_id, RHI_Shader_Type shader_type, core::RHI_Vertex_Type vertex_type, const std::string& shader_name );

		SHADER_D3D11* get_shader( RendererShader shader_id ) const;

		TEXTURE2D_BASE* create_texture2d( uint32_t width, uint32_t height, RHI_Format format, uint16_t flags, const std::vector<std::byte>& data );;

		SWAPCHAIN_BASE* create_swapchain( WINDOW_ID_TYPE window_id, float width, float height, RHI_Format format, const std::string& name );

		void pass_main();

		void pass_update_framebuffer( WINDOW_ID_TYPE window_id );

		void pass_gbuffer( WINDOW_ID_TYPE window_id );

		void pass_copy_to_framebuffer( WINDOW_ID_TYPE window_id );

		void present();

		const RENDERING_DEVICE_D3D11& get_device() const { return m_device; }
		RENDERING_DEVICE_D3D11& get_device() { return m_device; }

		TEXTURE2D_D3D11* get_render_target( const RENDER_TARGET render_target ) const { return m_render_targets.at( render_target ).get(); }

		TEXTURE2D_D3D11* get_viewport( WIDGET* viewport_widget ) const;

	//private:
		void on_mesh_created( engine::MESH* mesh ) override;

		void update_global_constant_buffer() override;

		void update_frame_constant_buffer() override;

		void create_shaders();

		void create_constant_buffers();

		void create_render_targets();

		void set_samplers();

		bool update_framebuffer();

		std::unordered_map< RendererShader, std::unique_ptr<SHADER_D3D11> > m_shaders;
		std::unordered_map< RENDER_TARGET, std::unique_ptr<TEXTURE2D_D3D11> > m_render_targets;
		std::unordered_map<WINDOW_ID_TYPE, WINDOW_CONTEXT_D3D11> m_window_contexts;

		RENDERING_DEVICE_D3D11 m_device;
		COMMAND_LIST_D3D11 m_command_list;
		
		BUFFER_D3D11 m_global_constant_buffer;
		BUFFER_D3D11 m_frame_constant_buffer;

		SAMPLER_D3D11 m_sampler_point_wrap;
		RASTERISER_STATE_D3D11 m_rasterizer_cull_back_solid;
	};
}
