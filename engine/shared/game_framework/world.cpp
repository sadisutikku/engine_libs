//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/world.h"

#include "core/shared/patterns/instance_tracker.h"

namespace engine
{
	WORLD::WORLD()
	{
		m_node_type = engine::NODE_BRANCH_TYPE::WORLD_ROOT;

		s_current_world = this;
		s_current_level = &m_levels.emplace_back();

		core::INSTANCE_TRACKER<WORLD>::add_instance( this );
	}

	WORLD::~WORLD()
	{
		core::INSTANCE_TRACKER<WORLD>::remove_instance( this );
	}

	bool WORLD::receive_event( const engine::EVENT& in_event )
	{
		auto handled{ false };

		switch ( in_event.unique_id )
		{
		case TICK_EVENT_ID:
			{
				
			}
			break;
		}

		return handled;
	}

	WORLD_OBJECT& WORLD::create_world_object( const std::string& mesh_filepath, gui::MATERIAL* material )
	{
		WORLD_OBJECT& world_object = s_current_level->m_world_objects.emplace_back();
		world_object.create_mesh( { {0.0f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f} }, { 0, 1, 2 }, material );
		m_on_mesh_created.m_inner.execute<void, MESH*>( world_object.get_mesh() );

		return world_object;
	}

	WORLD_OBJECT& WORLD::create_world_object( std::vector<core::RHI_Vertex_PosTexNorTan>&& vertices, std::vector<uint32_t>&& indices, 
											  gui::MATERIAL* material )
	{
		WORLD_OBJECT& world_object = s_current_level->m_world_objects.emplace_back();
		world_object.create_mesh( std::move( vertices ), std::move( indices ), material );
		m_on_mesh_created.m_inner.execute<void, MESH*>( world_object.get_mesh() );

		return world_object;
	}

	core::VECTOR<WORLD_OBJECT*> WORLD::get_world_objects()
	{
		core::VECTOR<WORLD_OBJECT*> objects;

		for ( auto& level : m_levels )
		{
			for ( auto& object : level.m_world_objects )
			{
				objects.push_back( &object );
			}
		}

		return objects;
	}

	WORLD* WORLD::get_world()
	{
		return s_current_world;
	}
}