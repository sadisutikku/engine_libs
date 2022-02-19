
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/window/window_details.h"

#include "core/shared/debugging/asserts.h"

#include <gui/thirdparty/imgui/imgui.h>
#include <gui/thirdparty/imgui/imgui_internal.h>

#if PLATFORM_WINDOWS
#include "core/platforms/include_wrappers/windows/windows.h"
#include <gui/thirdparty/imgui/windows/imgui_impl_win32.h>
#include <gui/thirdparty/imgui/windows/imgui_impl_dx11.h>
#endif

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace gui
{
	class RENDERER;
	class SWAPCHAIN_BASE;
	class SWAPCHAIN_D3D11;
	class WINDOW;
}

struct ImDrawData;
struct ImGuiViewport;
struct ImVec2;

namespace gui::imgui
{
	void initialise( RENDERER& renderer, WINDOW& window );

	inline void shutdown()
	{
		//ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		ImGui::DestroyPlatformWindows();
	}

	void render( ImDrawData* draw_data, SWAPCHAIN_D3D11* swapchain );

	void platform_create_window( ImGuiViewport* viewport );

	void RHI_Window_Create( ImGuiViewport* viewport );

	void RHI_Window_SetSize( ImGuiViewport* viewport, const ImVec2 size );

	void RHI_Window_Render( ImGuiViewport* viewport, void* );

	void RHI_Window_Present( ImGuiViewport* viewport, void* );

	void RHI_Window_Destroy( ImGuiViewport* viewport );

	inline void initialise_platform_interface()
	{
		ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

		platform_io.Platform_CreateWindow = platform_create_window;

		platform_io.Renderer_CreateWindow = RHI_Window_Create;
		platform_io.Renderer_DestroyWindow = RHI_Window_Destroy;
		platform_io.Renderer_SetWindowSize = RHI_Window_SetSize;
		platform_io.Renderer_RenderWindow = RHI_Window_Render;
		platform_io.Renderer_SwapBuffers = RHI_Window_Present;
	}
}
