
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/renderer/renderer_base.h"

#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
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
	class DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN;
	class PIPELINE_VULKAN;
	class SHADER_VULKAN;
	class SWAPCHAIN_VULKAN;
	enum class RendererShader : uint8_t;

	struct WINDOW_CONTEXT_VULKAN : public WINDOW_CONTEXT
	{
		std::unique_ptr<SWAPCHAIN_VULKAN> swapchain;
		//std::unordered_map<void*, VIEWPORT_DETAILS> viewports;
	};

	class RENDERER_VULKAN : public RENDERER_BASE
	{
	public:
		RENDERER_VULKAN();
		~RENDERER_VULKAN();

		void setup_window( WINDOW_ID_TYPE window_id );

		void pass_main();

		void pass_gbuffer( WINDOW_ID_TYPE window_id );

		void present();

		void on_mesh_created( engine::MESH* ) override {}

		void update_global_constant_buffer() override {}

		void update_frame_constant_buffer() override {}

		const RENDERING_DEVICE_VULKAN& get_device() const { return m_device; }
		RENDERING_DEVICE_VULKAN& get_device() { return m_device; }

	private:
		void create_shaders();

		std::unique_ptr<DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN> m_descriptor_set_layout_cache;
		std::unique_ptr<PIPELINE_VULKAN> m_pipeline;
		std::unordered_map< RendererShader, std::unique_ptr<SHADER_VULKAN> > m_shaders;
		std::unordered_map<WINDOW_ID_TYPE, WINDOW_CONTEXT_VULKAN> m_window_contexts;
		RENDERING_DEVICE_VULKAN m_device;
	};
}
