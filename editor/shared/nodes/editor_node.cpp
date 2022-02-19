
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/nodes/editor_node.h"

#include "editor/shared/events/editor_events.h"
#include "editor/shared/workspaces/editors/editor_base.h"
#include "editor/shared/workspaces/editors/editor_assets.h"
#include "editor/shared/workspaces/editors/editor_properties.h"
#include "editor/shared/workspaces/editors/editor_scene_tree.h"
#include "editor/shared/workspaces/editors/editor_toolbar.h"
#include "editor/shared/workspaces/editors/editor_viewport.h"
#include "editor/shared/workspaces/editors/editor_world.h"
#include "engine/shared/events/events_manager.h"
#include "gui/shared/events/render_events.h"
#include "gui/shared/imgui/imgui.h"
#include "gui/shared/graphics/renderer/renderer.h"
#include "gui/shared/screen/screen_manager.h"
#include "input/shared/input_events.h"

#include "gui/platforms/graphics/renderer/windows/renderer_d3d11.h" // temp
#include <d3d11.h> // temp
#include "gui/thirdparty/imgui/windows/imgui_impl_dx11.h" // temp

namespace
{
	// Events that can be received from an EVENT_MANAGER
	const core::VECTOR<core::UNIQUE_ID> s_broadcastable_events
	{
		{ 
			engine::ADD_NODE_EVENT_ID,
			input::MOUSE_MOVE_EVENT_ID,
			input::MOUSE_SCROLL_EVENT_ID,
			input::MOUSE_BUTTON_PRESSED_EVENT_ID,
			input::KEY_PRESSED_EVENT_ID,
		}
	};
}

namespace editor
{
	EDITOR_NODE::EDITOR_NODE()
	{
		m_node_type = engine::NODE_BRANCH_TYPE::EDITOR_ROOT;

		engine::EVENTS_MANAGER::get_singleton()->add_events_receiver( this, s_broadcastable_events );

		setup_imgui();

		m_children.emplace_back( new EDITOR_ASSETS );
		m_children.emplace_back( new EDITOR_PROPERTIES );
		m_children.emplace_back( new EDITOR_SCENE_TREE );
		//m_children.emplace_back( new EDITOR_WORLD );
		m_children.emplace_back( new EDITOR_VIEWPORT );
		m_children.emplace_back( new EDITOR_TOOLBAR );
	}

	EDITOR_NODE::~EDITOR_NODE()
	{
		shutdown_imgui();

		engine::EVENTS_MANAGER::get_singleton()->remove_events_receiver( this );
	}

	bool EDITOR_NODE::receive_event( const engine::EVENT& in_event )
	{
		auto handled{ false };

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{
				// Process the rest of the tree...
				handled = NODE::receive_event( in_event );

				// ...then process editor-specific render data
				receive_event( { gui::RENDER_IMGUI_EVENT_ID } );
			}
			break;
		case input::MOUSE_MOVE_EVENT_ID:
		case input::MOUSE_SCROLL_EVENT_ID:
		case input::MOUSE_BUTTON_PRESSED_EVENT_ID:
		case input::KEY_PRESSED_EVENT_ID:
			{
				handled = NODE::receive_event( in_event );
			}
			break;
		case gui::RENDER_IMGUI_EVENT_ID:
			{
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				//const auto begun = begin_window();

				handled = NODE::receive_event( in_event );

				/*if ( ImGui::Begin( "Boo" ) )
				{
					ImGui::Text( "Hello, world %d", 123 );

					ImGui::End();
				}

				if ( ImGui::Begin( "hello" ) )
				{
					ImGui::Text( "Hello, world %d", 123 );

					ImGui::End();
				}

				if ( ImGui::Begin( "next" ) )
				{
					ImGui::Text( "Hello, world %d", 123 );

					ImGui::End();
				}*/

				ImGui::Render();

				auto swapchain = static_cast<gui::RENDERER_D3D11*>(gui::RENDERER::get_singleton()->m_impl.get())->m_window_contexts.at( gui::SCREEN_MANAGER::MAIN_SCREEN_ID ).swapchain.get();
				//swapchain->clear_backbuffer( { 1.0f, 0.0f, 0.0f } );
				gui::imgui::render( ImGui::GetDrawData(), swapchain );

				//swapchain->present();

				// ImGui - child windows
				if ( ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
				}
			}
			break;
		case engine::ADD_NODE_EVENT_ID:
			{
				auto add_node_event = static_cast<const engine::ADD_NODE_EVENT&>(in_event);
				if ( add_node_event.parent && add_node_event.child )
				{
					add_node_event.parent->add_child( add_node_event.child );
				}
			}
			break;
		}

		return handled;
	}

	void EDITOR_NODE::setup_imgui()
	{
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
		io.ConfigWindowsResizeFromEdges = true;
		io.ConfigViewportsNoTaskBarIcon = true;
		apply_style();
		
		auto window = gui::SCREEN_MANAGER::get_singleton()->get_window( gui::SCREEN_MANAGER::MAIN_SCREEN_ID );
		gui::imgui::initialise( *gui::RENDERER::get_singleton(), *window );
	}

	void EDITOR_NODE::apply_style()
	{
		// Color settings
		const auto color_text = ImVec4( 0.810f, 0.810f, 0.810f, 1.000f );
		const auto color_text_disabled = ImVec4( color_text.x, color_text.y, color_text.z, 0.5f );
		const auto color_interactive = ImVec4( 0.338f, 0.338f, 0.338f, 1.000f );
		const auto color_interactive_hovered = ImVec4( 0.450f, 0.450f, 0.450f, 1.000f );
		const auto color_interactive_clicked = ImVec4( 0.586f, 0.586f, 0.586f, 1.000f );
		const auto color_background = ImVec4( 50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f );
		const auto color_background_content = ImVec4( 35.0f / 255.0f, 35.0f / 255.0f, 35.0f / 255.0f, 1.0f );
		const auto color_shadow = ImVec4( 0.0f, 0.0f, 0.0f, 0.5f );

		// Use default dark style as a base
		ImGui::StyleColorsDark();

		// Colors
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = color_text;
		colors[ImGuiCol_TextDisabled] = color_text_disabled;
		colors[ImGuiCol_WindowBg] = color_background;             // Background of normal windows
		colors[ImGuiCol_ChildBg] = color_background;             // Background of child windows
		colors[ImGuiCol_PopupBg] = color_background;             // Background of popups, menus, tooltips windows
		colors[ImGuiCol_Border] = color_interactive;
		colors[ImGuiCol_BorderShadow] = color_shadow;
		colors[ImGuiCol_FrameBg] = color_background_content;     // Background of checkbox, radio button, plot, slider, text input
		colors[ImGuiCol_FrameBgHovered] = color_interactive;
		colors[ImGuiCol_FrameBgActive] = color_interactive_clicked;
		colors[ImGuiCol_TitleBg] = color_background_content;
		colors[ImGuiCol_TitleBgActive] = color_interactive;
		colors[ImGuiCol_TitleBgCollapsed] = color_background;
		colors[ImGuiCol_MenuBarBg] = color_background_content;
		colors[ImGuiCol_ScrollbarBg] = color_background_content;
		colors[ImGuiCol_ScrollbarGrab] = color_interactive;
		colors[ImGuiCol_ScrollbarGrabHovered] = color_interactive_hovered;
		colors[ImGuiCol_ScrollbarGrabActive] = color_interactive_clicked;
		colors[ImGuiCol_CheckMark] = color_text;
		colors[ImGuiCol_SliderGrab] = color_interactive;
		colors[ImGuiCol_SliderGrabActive] = color_interactive_clicked;
		colors[ImGuiCol_Button] = color_interactive;
		colors[ImGuiCol_ButtonHovered] = color_interactive_hovered;
		colors[ImGuiCol_ButtonActive] = color_interactive_clicked;
		colors[ImGuiCol_Header] = color_interactive;            // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		colors[ImGuiCol_HeaderHovered] = color_interactive_hovered;
		colors[ImGuiCol_HeaderActive] = color_interactive_clicked;
		colors[ImGuiCol_Separator] = color_interactive;
		colors[ImGuiCol_SeparatorHovered] = color_interactive_hovered;
		colors[ImGuiCol_SeparatorActive] = color_interactive_clicked;
		colors[ImGuiCol_ResizeGrip] = color_interactive;
		colors[ImGuiCol_ResizeGripHovered] = color_interactive_hovered;
		colors[ImGuiCol_ResizeGripActive] = color_interactive_clicked;
		colors[ImGuiCol_Tab] = color_interactive;
		colors[ImGuiCol_TabHovered] = color_interactive_hovered;
		colors[ImGuiCol_TabActive] = color_interactive_clicked;
		colors[ImGuiCol_TabUnfocused] = color_interactive;
		colors[ImGuiCol_TabUnfocusedActive] = color_interactive;            // Might be called active, but it's active only because it's it's the only tab available, the user didn't really activate it
		colors[ImGuiCol_DockingPreview] = color_interactive_clicked;    // Preview overlay color when about to docking something
		colors[ImGuiCol_DockingEmptyBg] = color_interactive;            // Background color for empty node (e.g. CentralNode with no window docked into it)
		colors[ImGuiCol_PlotLines] = color_interactive;
		colors[ImGuiCol_PlotLinesHovered] = color_interactive_hovered;
		colors[ImGuiCol_PlotHistogram] = color_interactive;
		colors[ImGuiCol_PlotHistogramHovered] = color_interactive_hovered;
		colors[ImGuiCol_TextSelectedBg] = color_background;
		colors[ImGuiCol_DragDropTarget] = color_interactive_hovered;    // Color when hovering over target
		colors[ImGuiCol_NavHighlight] = color_background;             // Gamepad/keyboard: current highlighted item
		colors[ImGuiCol_NavWindowingHighlight] = color_background;             // Highlight window when using CTRL+TAB
		colors[ImGuiCol_NavWindowingDimBg] = color_background;             // Darken/colorize entire screen behind the CTRL+TAB window list, when active
		colors[ImGuiCol_ModalWindowDimBg] = color_background;             // Darken/colorize entire screen behind a modal window, when one is active

		// Spatial settings
		constexpr auto font_size = 24.0f;
		constexpr auto font_scale = 0.7f;
		constexpr auto roundness = 2.0f;

		// Spatial
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 1.0f;
		style.FrameBorderSize = 0.0f;
		style.ScrollbarSize = 20.0f;
		style.FramePadding = ImVec2( 5, 5 );
		style.ItemSpacing = ImVec2( 6, 5 );
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.WindowRounding = roundness;
		style.FrameRounding = roundness;
		style.PopupRounding = roundness;
		style.GrabRounding = roundness;
		style.ScrollbarRounding = roundness;
		style.Alpha = 1.0f;

		// Font
		auto& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF( "C:/dev/Silver/data/fonts/CalibriBold.ttf", font_size );
		io.FontGlobalScale = font_scale;
	}

	bool EDITOR_NODE::begin_window()
	{
		// Set window flags
		constexpr auto window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// Set window position and size
		float offset_y{};// = _editor::widget_menu_bar ? (_editor::widget_menu_bar->GetHeight() + _editor::widget_menu_bar->GetPadding()) : 0;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos( ImVec2( viewport->Pos.x, viewport->Pos.y + offset_y ) );
		ImGui::SetNextWindowSize( ImVec2( viewport->Size.x, viewport->Size.y - offset_y ) );
		ImGui::SetNextWindowViewport( viewport->ID );

		// Set window style
		ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
		ImGui::SetNextWindowBgAlpha( 0.0f );

		// Begin window
		bool open = true;
		bool editor_begun = ImGui::Begin( "editor", &open, window_flags );
		ImGui::PopStyleVar( 3 );

		// Begin dock space
		if ( ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable && editor_begun )
		{
			// Dock space
			const auto window_id = ImGui::GetID( "editor" );
			if ( !ImGui::DockBuilderGetNode( window_id ) )
			{
				// Reset current docking state
				ImGui::DockBuilderRemoveNode( window_id );
				ImGui::DockBuilderAddNode( window_id, ImGuiDockNodeFlags_None );
				ImGui::DockBuilderSetNodeSize( window_id, ImGui::GetMainViewport()->Size );

				// DockBuilderSplitNode(ImGuiID node_id, ImGuiDir split_dir, float size_ratio_for_node_at_dir, ImGuiID* out_id_dir, ImGuiID* out_id_other);
				ImGuiID dock_main_id = window_id;
				ImGuiID dock_right_id = ImGui::DockBuilderSplitNode( dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id );
				const ImGuiID dock_right_down_id = ImGui::DockBuilderSplitNode( dock_right_id, ImGuiDir_Down, 0.6f, nullptr, &dock_right_id );
				ImGuiID dock_down_id = ImGui::DockBuilderSplitNode( dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id );
				const ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode( dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id );

				// Dock windows
				ImGui::DockBuilderDockWindow( "World", dock_right_id );
				ImGui::DockBuilderDockWindow( "Properties", dock_right_down_id );
				ImGui::DockBuilderDockWindow( "Console", dock_down_id );
				ImGui::DockBuilderDockWindow( "Assets", dock_down_right_id );
				ImGui::DockBuilderDockWindow( "Viewport", dock_main_id );

				ImGui::DockBuilderFinish( dock_main_id );
			}

			ImGui::DockSpace( window_id, ImVec2( 0.0f, 0.0f ), ImGuiDockNodeFlags_PassthruCentralNode );
		}

		return editor_begun;
	}

	void EDITOR_NODE::shutdown_imgui()
	{
		if ( ImGui::GetCurrentContext() )
		{
			gui::imgui::shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}
	}
}
