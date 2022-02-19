//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/vertex.h"

#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace gui
{
	class MATERIAL;
}

namespace engine
{
	class MESH
	{
	public:
		MESH( const std::vector<core::RHI_Vertex_PosTexNorTan>&& vertices, const std::vector<uint32_t>&& indices, gui::MATERIAL* material = nullptr );
		~MESH() = default;

		const std::vector<core::RHI_Vertex_PosTexNorTan>& get_vertices() const { return m_vertices; }

		const std::vector<uint32_t>& get_indices() const { return m_indices; }

		gui::MATERIAL* m_material{};

	private:
		std::vector<core::RHI_Vertex_PosTexNorTan> m_vertices;
		std::vector<uint32_t> m_indices;
	};
}