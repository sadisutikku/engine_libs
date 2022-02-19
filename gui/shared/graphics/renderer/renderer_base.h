
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/type_wrappers/vector.h"
#include "gui/shared/screen/screen_area.h"
#include "gui/shared/graphics/constant_buffers.h"

#include <memory>

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace core
{
	class MATRIX;
}

namespace engine
{
	class MESH;
	class WORLD;
}

namespace gui
{
	class RENDERING_DEVICE_BASE;
	enum class BUFFER_TYPE : uint8_t;

	struct WINDOW_CONTEXT
	{
		core::VECTOR<SCREEN_AREA> screen_areas;
	};

	class RENDERER_BASE
	{
	public:
		RENDERER_BASE();
		~RENDERER_BASE();

		void set_global_shader_object_transform( const core::MATRIX& transform );

	//protected:
		template<typename BUFFER, typename VERTEX>
		BUFFER* create_buffer( RENDERING_DEVICE_BASE& device, BUFFER_TYPE buffer_type, const std::vector<VERTEX>& data )
		{
			auto buffer{ std::make_unique<BUFFER>() };
			if ( buffer->create( device, buffer_type, data ) )
			{
				return buffer.release();
			}
			else
			{
				return nullptr;
			}
		}

		template<typename BUFFER, typename VERTEX>
		BUFFER* create_dynamic_buffer( RENDERING_DEVICE_BASE& device, BUFFER_TYPE buffer_type, const uint32_t vertex_count )
		{
			auto buffer{ std::make_unique<BUFFER>() };
			if ( buffer->create_dynamic<VERTEX>( device, buffer_type, vertex_count ) )
			{
				return buffer.release();
			}
			else
			{
				return nullptr;
			}
		}

		virtual void on_mesh_created( engine::MESH* ) = 0;

		virtual void update_global_constant_buffer() = 0;

		virtual void update_frame_constant_buffer() = 0;

		GLOBAL_CONSTANT_BUFFER m_global_constant_buffer_cpu;
		FRAME_CONSTANT_BUFFER m_frame_constant_buffer_cpu;

	private:
		void on_world_created( engine::WORLD* world );
				
		size_t m_on_world_created_id{};
	};
}
