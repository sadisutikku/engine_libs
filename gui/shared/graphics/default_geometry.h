
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/vector2d.h"
#include "core/shared/math/vector3d.h"
#include "core/shared/type_wrappers/vector.h"
#include "core/shared/vertex.h"

namespace gui
{
	inline void create_cube( std::vector<core::RHI_Vertex_PosTexNorTan>& vertices, std::vector<uint32_t>& indices )
	{
		vertices =
		{
			// front
			{ { -0.5f, -0.5f, -0.5f }, { 0, 1 }, { 0, 0, -1 }, { 0, 1, 0 } },
			{ { -0.5f, 0.5f, -0.5f }, { 0, 0 }, { 0, 0, -1 }, { 0, 1, 0 } },
			{ { 0.5f, -0.5f, -0.5f }, { 1, 1 }, { 0, 0, -1 }, { 0, 1, 0 } },
			{ { 0.5f, 0.5f, -0.5f }, { 1, 0 }, { 0, 0, -1 }, { 0, 1, 0 } },

			// bottom
			{ { -0.5f, -0.5f, 0.5f }, { 0, 1 }, { 0, -1, 0 }, { 1, 0, 0 } },
			{ { -0.5f, -0.5f, -0.5f }, { 0, 0 }, { 0, -1, 0 }, { 1, 0, 0 } },
			{ { 0.5f, -0.5f, 0.5f }, { 1, 1 }, { 0, -1, 0 }, { 1, 0, 0 } },
			{ { 0.5f, -0.5f, -0.5f }, { 1, 0 }, { 0, -1, 0 }, { 1, 0, 0 } },

			// back
			{ { -0.5f, -0.5f, 0.5f }, { 1, 1 }, { 0, 0, 1 }, { 0, 1, 0 } },
			{ { -0.5f, 0.5f, 0.5f }, { 1, 0 }, { 0, 0, 1 }, { 0, 1, 0 } },
			{ { 0.5f, -0.5f, 0.5f }, { 0, 1 }, { 0, 0, 1 }, { 0, 1, 0 } },
			{ { 0.5f, 0.5f, 0.5f }, { 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 } },

			// top
			{ { -0.5f, 0.5f, 0.5f }, { 0, 0 }, { 0, 1, 0 }, { 1, 0, 0 } },
			{ { -0.5f, 0.5f, -0.5f }, { 0, 1 }, { 0, 1, 0 }, { 1, 0, 0 } },
			{ { 0.5f, 0.5f, 0.5f }, { 1, 0 }, { 0, 1, 0 }, { 1, 0, 0 } },
			{ { 0.5f, 0.5f, -0.5f }, { 1, 1 }, { 0, 1, 0 }, { 1, 0, 0 } },

			// left
			{ { -0.5f, -0.5f, 0.5f }, { 0, 1 }, { -1, 0, 0 }, { 0, 1, 0 } },
			{ { -0.5f, 0.5f, 0.5f }, { 0, 0 }, { -1, 0, 0 }, { 0, 1, 0 } },
			{ { -0.5f, -0.5f, -0.5f }, { 1, 1 }, { -1, 0, 0 }, { 0, 1, 0 } },
			{ { -0.5f, 0.5f, -0.5f }, { 1, 0 }, { -1, 0, 0 }, { 0, 1, 0 } },

			// right
			{ { 0.5f, -0.5f, 0.5f }, { 1, 1 }, { 1, 0, 0 }, { 0, 1, 0 } },
			{ { 0.5f, 0.5f, 0.5f }, { 1, 0 }, { 1, 0, 0 }, { 0, 1, 0 } },
			{ { 0.5f, -0.5f, -0.5f }, { 0, 1 }, { 1, 0, 0 }, { 0, 1, 0 } },
			{ { 0.5f, 0.5f, -0.5f }, { 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 } },
		};

		// front
		indices.emplace_back( 0 ); indices.emplace_back( 1 ); indices.emplace_back( 2 );
		indices.emplace_back( 2 ); indices.emplace_back( 1 ); indices.emplace_back( 3 );

		// bottom
		indices.emplace_back( 4 ); indices.emplace_back( 5 ); indices.emplace_back( 6 );
		indices.emplace_back( 6 ); indices.emplace_back( 5 ); indices.emplace_back( 7 );

		// back
		indices.emplace_back( 10 ); indices.emplace_back( 9 ); indices.emplace_back( 8 );
		indices.emplace_back( 11 ); indices.emplace_back( 9 ); indices.emplace_back( 10 );

		// top
		indices.emplace_back( 14 ); indices.emplace_back( 13 ); indices.emplace_back( 12 );
		indices.emplace_back( 15 ); indices.emplace_back( 13 ); indices.emplace_back( 14 );

		// left
		indices.emplace_back( 16 ); indices.emplace_back( 17 ); indices.emplace_back( 18 );
		indices.emplace_back( 18 ); indices.emplace_back( 17 ); indices.emplace_back( 19 );

		// right
		indices.emplace_back( 22 ); indices.emplace_back( 21 ); indices.emplace_back( 20 );
		indices.emplace_back( 23 ); indices.emplace_back( 21 ); indices.emplace_back( 22 );
	}

	inline void create_quad( std::vector<core::RHI_Vertex_PosTexNorTan>& vertices, std::vector<uint32_t>& indices )
	{
		vertices =
		{
			{ { -0.5f, 0.5f, 0.0f }, { 0, 0 },{ 0, 1, 0 },{ 1, 0, 0 } }, // 0 top-left
			{ { 0.5f, 0.5f, 0.0f }, { 1, 0 },{ 0, 1, 0 },{ 1, 0, 0 } }, // 1 top-right
			{ { -0.5f, -0.5f, 0.0f }, { 0, 1 },{ 0, 1, 0 },{ 1, 0, 0 }  }, // 2 bottom-left
			{ { 0.5f, -0.5f, 0.0f }, { 1, 1 },{ 0, 1, 0 },{ 1, 0, 0 } } // 3 bottom-right
		};

		indices.emplace_back( 3 );
		indices.emplace_back( 2 );
		indices.emplace_back( 0 );
		indices.emplace_back( 3 );
		indices.emplace_back( 0 );
		indices.emplace_back( 1 );
	}
}
