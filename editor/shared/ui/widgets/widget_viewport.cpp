
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/ui/widgets/widget_viewport.h"

#include "core/shared/math/rectangle.h"
#include "core/shared/patterns/scoped_value.h"
#include "editor/shared/events/editor_events.h"
#include "engine/shared/application/application.h"
#include "engine/shared/game_framework/game_instance.h"
#include "engine/shared/game_framework/nodes/node_tree.h"
#include "engine/shared/game_framework/world.h"
#include "gui/shared/events/render_events.h"
#include "gui/shared/graphics/renderer/renderer.h"
#include "gui/shared/graphics/RHI/texture2d_base.h"
#include "gui/shared/imgui/imgui.h"
#include "input/shared/input_events.h"

namespace editor
{
	WIDGET_VIEWPORT::WIDGET_VIEWPORT()
	{
		static uint32_t unique_counter{ 0 };
		m_title = "Viewport" + std::to_string( unique_counter++ );
		m_flags |= ImGuiWindowFlags_NoScrollbar;

		m_max_size = { 1000.0f, 800.0f };
		m_bounds.set_dimensions( { 800.0f, 600.0f } );

		gui::RENDERER::get_singleton()->register_viewport( this, &m_camera3d, engine::WORLD::get_world() /*m_world*/ );
	}

	bool WIDGET_VIEWPORT::receive_event( const engine::EVENT& in_event )
	{
		bool handled{ false };

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{
				handled = WIDGET::receive_event( in_event );
			}
			break;
		case input::MOUSE_BUTTON_PRESSED_EVENT_ID:
			{
				if ( WIDGET::receive_event( in_event ) )
				{
					auto button_pressed_event = static_cast<const input::MOUSE_BUTTON_PRESSED_EVENT&>(in_event);

					if ( button_pressed_event.mouse_button == input::MOUSE_BUTTON::RIGHT )
					{
						m_allow_camera_movement = button_pressed_event.pressed;
					}
				}
			}
			break;
		case input::MOUSE_MOVE_EVENT_ID:
			{
				if ( m_allow_camera_movement && WIDGET::receive_event( in_event ) )
				{
					auto mouse_move_event = static_cast<const input::MOUSE_MOVE_EVENT&>(in_event);

					if ( mouse_move_event.delta != input::MOUSE_POS_TYPE::ZERO_VALUE )
					{
						m_camera3d.rotate( { static_cast<float>(mouse_move_event.delta.x), static_cast<float>(mouse_move_event.delta.y) } );
					}
				}
			}
			break;
		case input::KEY_PRESSED_EVENT_ID:
			{
				auto key_pressed_event = static_cast<const input::KEY_PRESSED_EVENT&>(in_event);

				if ( m_allow_camera_movement && key_pressed_event.pressed )
				{
					switch ( key_pressed_event.key )
					{
					case input::KEY::W:
						{
							m_camera3d.move( { 0.0f, 0.0f, 1.0f } );
						}
						break;
					case input::KEY::S:
						{
							m_camera3d.move( { 0.0f, 0.0f, -1.0f } );
						}
						break;
					case input::KEY::A:
						{
							m_camera3d.move( { -1.0f, 0.0f, 0.0f } );
						}
						break;
					case input::KEY::D:
						{
							m_camera3d.move( { 1.0f, 0.0f, 0.0f } );
						}
						break;
					case input::KEY::Q:
						{
							m_camera3d.move({ 0.0f, 1.0f, 0.0f });
						}
						break;
					case input::KEY::E:
						{
							m_camera3d.move({ 0.0f, -1.0f, 0.0f });
						}
						break;
					}
				}
			}
			break;
		case gui::MARK_FOR_RENDER_EVENT_ID:
			{
				handled = WIDGET::receive_event( in_event );
			}
			break;
		case gui::RENDER_IMGUI_EVENT_ID:
			{
				if ( m_should_render )
				{
					handled = WIDGET::receive_event( in_event );
				}
			}
			break;
		}

		return handled;
	}

	void WIDGET_VIEWPORT::render()
	{
		if ( m_is_visible )
		{
			float width = static_cast<float>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
			float height = static_cast<float>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

			//core::FVECTOR2D offset{ ImGui::GetWindowPos().x + m_padding.x, ImGui::GetWindowPos().y + m_padding.y };

			//auto renderer = gui::RENDERER::get_singleton();

			//if ( m_bounds.get_width() != width || m_bounds.get_height() != height)
			//{
			//	//m_renderer->SetViewport( width, height, offset.x, offset.y );

			//	m_bounds.set_dimensions( { width, height } );
			//}

			//auto viewport_texture = renderer->get_render_target( gui::RENDER_TARGET::REGION_TEST_0 )->get_resource_view();
			auto viewport_texture = gui::RENDERER::get_singleton()->get_viewport( this )->get_resource_view();

			ImGui::Image(
				static_cast<ImTextureID>(viewport_texture),
				{ width, height },
				ImVec2( 0, 0 ),
				ImVec2( 1, 1 ),
				ImColor( 255, 255, 255, 255 ),
				ImColor( 50, 127, 166, 255 )
			);
		}
	}
}
