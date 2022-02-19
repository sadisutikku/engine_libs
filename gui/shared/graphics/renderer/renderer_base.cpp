
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/renderer/renderer_base.h"

#include "core/shared/patterns/instance_tracker.h"
#include "engine/shared/game_framework/world.h"

namespace gui
{
	RENDERER_BASE::RENDERER_BASE()
	{
		m_on_world_created_id = core::INSTANCE_TRACKER<engine::WORLD>::m_on_added_delegate.m_inner.add_function( [this]( engine::WORLD* world)
																						 {
																							 on_world_created( world );
																						 } );
	}

	RENDERER_BASE::~RENDERER_BASE()
	{
		core::INSTANCE_TRACKER<engine::WORLD>::m_on_added_delegate.m_inner.remove_function( m_on_world_created_id );
	}

	void RENDERER_BASE::set_global_shader_object_transform( const core::MATRIX& transform )
	{
		m_global_constant_buffer_cpu.transform = transform;
		update_global_constant_buffer();
	}

	void RENDERER_BASE::on_world_created( engine::WORLD* world )
	{
		world->m_on_mesh_created.m_inner.add_function( [this]( engine::MESH* mesh )
													   {
														   on_mesh_created( mesh );
													   } );
	}
}
