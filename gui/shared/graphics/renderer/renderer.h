
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/non_automatic_singleton.h"
#include "engine/shared/events/events_receiver.h"

#include "gui/shared/graphics/RHI/RHI_types.h"
#include "gui/shared/window/window_details.h"

#include <memory>
#include <string>
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
	class RENDERER_BASE;
	class SHADER_BASE;
	class SWAPCHAIN_BASE;
	class TEXTURE2D_BASE;
	class WIDGET;
	class CAMERA3D;
	enum class RendererShader : uint8_t;
	enum class RHI_Shader_Type : uint8_t;
	enum class RENDER_TARGET : uint8_t;
}

namespace gui
{
	class RENDERER : public engine::EVENTS_RECEIVER, public core::NON_AUTOMATIC_SINGLETON<RENDERER>
	{
	public:
		bool receive_event( const engine::EVENT& in_event ) override;

		//void tick( float delta_time );

		void setup_window( WINDOW_ID_TYPE window_id );

		void register_viewport( WIDGET* viewport_widget, CAMERA3D* camera3d, engine::WORLD* world );

		void create_shader( RendererShader shader_id, RHI_Shader_Type shader_type, core::RHI_Vertex_Type vertex_type, const std::string& shader_name );

		SHADER_BASE* get_shader( RendererShader shader_id ) const;

		TEXTURE2D_BASE* get_render_target( RENDER_TARGET render_target ) const;

		TEXTURE2D_BASE* get_viewport( WIDGET* viewport_widget ) const;

		TEXTURE2D_BASE* create_texture2d( uint32_t width, uint32_t height, RHI_Format format, uint16_t flags, const std::vector<std::byte>& data );

		SWAPCHAIN_BASE* create_swapchain( WINDOW_ID_TYPE window_id, float width, float height, RHI_Format format, const std::string& name );

		// Render Passes
		void pass_main();
		void pass_update_framebuffer();

		void present();
		
	//private:
		friend class core::NON_AUTOMATIC_SINGLETON<RENDERER>;
		RENDERER( RHI_Api_Type api_type );
		~RENDERER();

		std::unique_ptr<RENDERER_BASE> m_impl;
		RHI_Api_Type m_api_type{};
	};
}
