//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/nodes/node.h"

#include "core/shared/patterns/instance_tracker.h"
#include "core/shared/type_wrappers/vector.h"
#include "engine/shared/game_framework/level.h"

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace gui
{
	class MATERIAL;
}

namespace engine
{
	class MESH;

	inline constexpr auto PERSISTENT_LEVEL_INDEX{ 0 };

	class WORLD : public NODE
	{
	public:
		WORLD();
		~WORLD();

		bool receive_event( const engine::EVENT& in_event ) override;

		WORLD_OBJECT& create_world_object( const std::string& mesh_filepath, gui::MATERIAL* material );

		WORLD_OBJECT& create_world_object( std::vector<core::RHI_Vertex_PosTexNorTan>&& vertices, std::vector<uint32_t>&& indices, gui::MATERIAL* material );

		core::VECTOR<WORLD_OBJECT*> get_world_objects();

		static WORLD* get_world();

		inline static LEVEL* s_current_level{};
		inline static WORLD* s_current_world{};

		core::MULTICAST_DELEGATE_ONE_PARAM<void, MESH*> m_on_mesh_created;

		core::VECTOR<LEVEL> m_levels;
	};
}