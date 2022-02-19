
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/renderer/renderer.h"

#include "engine/shared/application/application.h"
#include "engine/shared/events/events_manager.h"
#include "gui/platforms/graphics/renderer/vulkan/renderer_vulkan.h"
#include "gui/platforms/graphics/renderer/windows/renderer_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/shader_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/texture2d_d3d11.h"
#include "gui/shared/graphics/storage/render_storage.h"

namespace
{
	// Events that can be received from an EVENT_MANAGER
	const core::VECTOR<core::UNIQUE_ID> s_broadcastable_events
	{
		{ engine::POST_APP_ITERATION_EVENT_ID }
	};
}

namespace gui
{
	RENDERER::RENDERER( const RHI_Api_Type api_type )
		: m_api_type{ api_type }
	{
		RENDER_STORAGE::create_singleton();

		engine::EVENTS_MANAGER::get_singleton()->add_events_receiver( this, s_broadcastable_events );

		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				m_impl = std::make_unique<RENDERER_D3D11>();
				break;
			}
		case RHI_Api_Type::RHI_Api_Vulkan:
			{
				m_impl = std::make_unique<RENDERER_VULKAN>();
				break;
			}
		}
	}

	RENDERER::~RENDERER()
	{
		engine::EVENTS_MANAGER::get_singleton()->remove_events_receiver( this );
	}

	bool RENDERER::receive_event( const engine::EVENT& in_event )
	{
		auto handled{ false };

		switch ( in_event.unique_id )
		{
		case engine::POST_APP_ITERATION_EVENT_ID:
			{
				present();
			}
			break;
		}

		return handled;
	}

	void RENDERER::setup_window( const WINDOW_ID_TYPE window_id )
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				static_cast<RENDERER_D3D11*>(m_impl.get())->setup_window( window_id );
				break;
			}
		case RHI_Api_Type::RHI_Api_Vulkan:
			{
				static_cast<RENDERER_VULKAN*>(m_impl.get())->setup_window( window_id );
				break;
			}
		}
	}

	void RENDERER::register_viewport( WIDGET* viewport_widget, CAMERA3D* camera3d, engine::WORLD* world )
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				static_cast<RENDERER_D3D11*>(m_impl.get())->register_viewport( viewport_widget, camera3d, world );
				break;
			}
		}
	}

	void RENDERER::create_shader( const RendererShader shader_id, const RHI_Shader_Type shader_type, 
								  const core::RHI_Vertex_Type vertex_type, const std::string& shader_name )
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				static_cast<RENDERER_D3D11*>(m_impl.get())->create_shader( shader_id, shader_type, vertex_type, shader_name );
				break;
			}
		}
	}

	SHADER_BASE* RENDERER::get_shader( const RendererShader shader_id ) const
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				return static_cast<SHADER_BASE*>(static_cast<RENDERER_D3D11*>(m_impl.get())->get_shader( shader_id ));
			}
		}

		return nullptr;
	}

	TEXTURE2D_BASE* RENDERER::get_render_target( const RENDER_TARGET render_target ) const
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				return static_cast<TEXTURE2D_BASE*>(static_cast<RENDERER_D3D11*>(m_impl.get())->get_render_target( render_target ));
			}
		}

		return nullptr;
	}

	TEXTURE2D_BASE* RENDERER::get_viewport( WIDGET* viewport_widget ) const
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				return static_cast<TEXTURE2D_BASE*>(static_cast<RENDERER_D3D11*>(m_impl.get())->get_viewport( viewport_widget ));
			}
		}

		return nullptr;
	}

	TEXTURE2D_BASE* RENDERER::create_texture2d( const uint32_t width, const uint32_t height, const RHI_Format format, const uint16_t flags, const std::vector<std::byte>& data )
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				return static_cast<RENDERER_D3D11*>(m_impl.get())->create_texture2d( width, height, format, flags, data );
			}
		}

		return nullptr;
	}

	SWAPCHAIN_BASE* RENDERER::create_swapchain( const  WINDOW_ID_TYPE window_id, const float width, const float height, const RHI_Format format, const std::string& name )
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				return static_cast<RENDERER_D3D11*>(m_impl.get())->create_swapchain( window_id, width, height, format, name );
			}
		}

		return nullptr;
	}

	void RENDERER::pass_main()
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				static_cast<RENDERER_D3D11*>(m_impl.get())->pass_main();
				break;
			}
		case RHI_Api_Type::RHI_Api_Vulkan:
			{
				static_cast<RENDERER_VULKAN*>(m_impl.get())->pass_main();
				break;
			}
		}
	}

	void RENDERER::pass_update_framebuffer()
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				//static_cast<RENDERER_D3D11*>(m_impl.get())->pass_update_framebuffer();
				break;
			}
		}
	}

	void RENDERER::present()
	{
		switch ( m_api_type )
		{
		case RHI_Api_Type::RHI_Api_D3d11:
			{
				static_cast<RENDERER_D3D11*>(m_impl.get())->present();
				break;
			}
		case RHI_Api_Type::RHI_Api_Vulkan:
			{
				static_cast<RENDERER_VULKAN*>(m_impl.get())->present();
				break;
			}
		}
	}
}
