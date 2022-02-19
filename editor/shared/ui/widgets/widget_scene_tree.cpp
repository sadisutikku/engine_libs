
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/ui/widgets/widget_scene_tree.h"

#include "core/shared/math/rectangle.h"
#include "core/shared/patterns/scoped_value.h"
#include "editor/shared/events/editor_events.h"
#include "engine/shared/application/application.h"
#include "engine/shared/events/events_manager.h"
#include "engine/shared/game_framework/game_instance.h"
#include "engine/shared/game_framework/nodes/node_tree.h"
#include "engine/shared/game_framework/world.h"
#include "gui/shared/events/render_events.h"
#include "gui/shared/graphics/default_geometry.h"
#include "gui/shared/graphics/material.h"
#include "gui/shared/imgui/imgui.h"
#include "input/shared/input_events.h"


namespace editor
{
	WIDGET_SCENE_TREE::WIDGET_SCENE_TREE()
	{
		m_title = "SceneTree";
		m_flags |= ImGuiWindowFlags_HorizontalScrollbar;

		m_max_size = { 1000.0f, 800.0f };
		m_bounds.set_dimensions( { 200.0f, 600.0f } );
	}

	bool WIDGET_SCENE_TREE::receive_event( const engine::EVENT& in_event )
	{
		bool handled{ false };

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{
				handled = WIDGET::receive_event( in_event );
			}
			break;
		case input::MOUSE_MOVE_EVENT_ID:
			{
				if ( WIDGET::receive_event( in_event ) )
				{
					receive_event( { gui::MARK_FOR_RENDER_EVENT_ID } );
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

	void WIDGET_SCENE_TREE::add_children_to_tree( NODE* node )
	{
		const auto& child_nodes = node->get_children();
		for ( size_t index{ 0 }; index < child_nodes.size(); ++index )
		{
			switch ( child_nodes[index]->get_node_type() )
			{
			case engine::NODE_BRANCH_TYPE::WORLD_ROOT:
				{
					auto world = static_cast<engine::WORLD*>(node);
					for ( auto object : world->get_world_objects() )
					{
						
					}
				}
				[[fallthrough]];
			case engine::NODE_BRANCH_TYPE::NONE:
				{
					add_node_to_tree( child_nodes[index].get() );
				}
				break;
			case engine::NODE_BRANCH_TYPE::ENGINE_ROOT:
			case engine::NODE_BRANCH_TYPE::GRAPHICS_ROOT:
			case engine::NODE_BRANCH_TYPE::EDITOR_ROOT:
				break;
			}
		}
	}

	void WIDGET_SCENE_TREE::add_node_to_tree( NODE* node )
	{
		if ( !node )
		{
			return;
		}

		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanFullWidth;
		node_flags |= node->has_children() ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf;

		if ( ImGui::TreeNodeEx( "gfd", node_flags ) )
		{
			if ( ImGui::IsItemHovered( ImGuiHoveredFlags_RectOnly ) )
			{
				
			}

			add_children_to_tree( node );

			ImGui::TreePop();
		}
	}

	void WIDGET_SCENE_TREE::render()
	{
		if ( m_is_visible )
		{
			static bool show_another_window{ false };
			//core::SCOPED_VALUE scoped_value{ m_should_render, false };

			if ( ImGui::TreeNodeEx( "Root", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth ) )
			{
				/*auto& game_instance = engine::APPLICATION::get_singleton()->get_main_game_instance();
				engine::NODE* root_node = game_instance.get_node_tree()->get_root_node();

				add_children_to_tree( root_node );*/

				const auto is_window_hovered = ImGui::IsWindowHovered( ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem );
				const auto left_click = ImGui::IsMouseClicked( 0 );
				const auto right_click = ImGui::IsMouseClicked( 1 );

				for ( auto object : engine::WORLD::get_world()->get_world_objects() )
				{
					ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf;

					if ( ImGui::TreeNodeEx( "object", node_flags ) )
					{
						if ( ImGui::IsItemHovered( ImGuiHoveredFlags_RectOnly ) && left_click )
						{
							engine::EVENTS_MANAGER::get_singleton()->add_event( std::make_unique<editor::SELECT_WORLD_OBJECT_EVENT>( object ) );
						}

						ImGui::TreePop();
					}
				}

				if ( ImGui::IsItemHovered( ImGuiHoveredFlags_RectOnly ) )
				{
					if ( is_window_hovered && right_click )
					{
						//ImGui::OpenPopup( "HierarchyContextMenu" );

						show_another_window = true;
					}
				}

				ImGui::TreePop();


				if ( show_another_window /*ImGui::BeginPopup( "HierarchyContextMenu" )*/ )
				{
					ImGui::Begin( "Another Window", &show_another_window );   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
					
					if ( ImGui::BeginMenu( "3D Objects" ) )
					{
						if ( ImGui::MenuItem( "Cube" ) )
						{
							std::vector<core::RHI_Vertex_PosTexNorTan> vertices;
							std::vector<uint32_t> indices;
							gui::create_cube( vertices, indices );

							auto material{ gui::MATERIAL::get_or_create_default_material() };
							engine::WORLD::get_world()->create_world_object( std::move( vertices ), std::move( indices ), material );
						}

						ImGui::EndMenu();
					}

					if ( ImGui::Button( "Close Me" ) )
					{
						show_another_window = false;
					}

					//ImGui::EndPopup();

					ImGui::End();
				}
			}
		}
	}
}
