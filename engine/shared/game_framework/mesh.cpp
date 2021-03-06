//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/mesh.h"

namespace engine
{
	MESH::MESH( const std::vector<core::RHI_Vertex_PosTexNorTan>&& vertices, const std::vector<uint32_t>&& indices, gui::MATERIAL* material )
	{
		m_vertices = std::move( vertices );
		m_indices = std::move( indices );
		m_material = material;
	}
}