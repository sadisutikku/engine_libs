
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/debugging/asserts.h"
#include "core/shared/vertex.h"
#include "gui/shared/graphics/RHI/RHI_types.h"

#include <string>
#include <vector>

namespace gui
{
	class RENDERING_DEVICE_BASE;

	struct VERTEX_ATTRIBUTE
	{
		std::string name;
		uint32_t location;
		uint32_t binding;
		RHI_Format format;
		uint32_t offset;
	};

	class INPUT_LAYOUT
	{
	public:
		bool create( RENDERING_DEVICE_BASE& device, const core::RHI_Vertex_Type vertex_type, void* vertex_shader_blob = nullptr )
		{
			if ( vertex_type == core::RHI_Vertex_Type::RHI_Vertex_Type_Unknown )
			{
				ASSERT_FAILED( "Unknown vertex type" );
				return false;
			}

			const uint32_t binding = 0;

			if ( vertex_type == core::RHI_Vertex_Type::RHI_Vertex_Type_Position )
			{
				m_vertex_attributes =
				{
					{ "POSITION", 0, binding, RHI_Format_R32G32B32_Float,    offsetof( core::RHI_Vertex_Pos, pos ) }
				};
			}

			if ( vertex_type == core::RHI_Vertex_Type::RHI_Vertex_Type_PositionTexture )
			{
				m_vertex_attributes =
				{
					{ "POSITION", 0, binding, RHI_Format_R32G32B32_Float,    offsetof( core::RHI_Vertex_PosTex, pos ) },
					{ "TEXCOORD", 1, binding, RHI_Format_R32G32_Float,        offsetof( core::RHI_Vertex_PosTex, tex ) }
				};
			}

			if ( vertex_type == core::RHI_Vertex_Type::RHI_Vertex_Type_PositionColor )
			{
				m_vertex_attributes =
				{
					{ "POSITION",    0, binding, RHI_Format_R32G32B32_Float,        offsetof( core::RHI_Vertex_PosCol, pos ) },
					{ "COLOR",        1, binding, RHI_Format_R32G32B32A32_Float,    offsetof( core::RHI_Vertex_PosCol, col ) }
				};
			}

			if ( vertex_type == core::RHI_Vertex_Type::RHI_Vertex_Type_Position2dTextureColor8 )
			{
				m_vertex_attributes =
				{
					{ "POSITION",    0, binding, RHI_Format_R32G32_Float,    offsetof( core::RHI_Vertex_Pos2dTexCol8, pos ) },
					{ "TEXCOORD",    1, binding, RHI_Format_R32G32_Float,    offsetof( core::RHI_Vertex_Pos2dTexCol8, tex ) },
					{ "COLOR",        2, binding, RHI_Format_R8G8B8A8_Unorm,    offsetof( core::RHI_Vertex_Pos2dTexCol8, col ) }
				};
			}

			if ( vertex_type == core::RHI_Vertex_Type::RHI_Vertex_Type_PositionTextureNormalTangent )
			{
				m_vertex_attributes =
				{
					{ "POSITION",    0, binding, RHI_Format_R32G32B32_Float,    offsetof( core::RHI_Vertex_PosTexNorTan, pos ) },
					{ "TEXCOORD",    1, binding, RHI_Format_R32G32_Float,    offsetof( core::RHI_Vertex_PosTexNorTan, tex ) },
					{ "NORMAL",        2, binding, RHI_Format_R32G32B32_Float,    offsetof( core::RHI_Vertex_PosTexNorTan, nor ) },
					{ "TANGENT",    3, binding, RHI_Format_R32G32B32_Float,    offsetof( core::RHI_Vertex_PosTexNorTan, tan ) }
				};
			}

			if ( vertex_shader_blob && !m_vertex_attributes.empty() )
			{
				return _CreateResource( device, vertex_shader_blob );
			}

			return true;
		}

		core::RHI_Vertex_Type GetVertexType()            const { return m_vertex_type; }
		const auto& GetAttributeDescriptions()    const { return m_vertex_attributes; }
		void* GetResource()                        const { return m_resource; }

		bool operator==( const INPUT_LAYOUT& rhs ) const { return m_vertex_type == rhs.GetVertexType(); }

	protected:
		core::RHI_Vertex_Type m_vertex_type;

		// API
		virtual bool _CreateResource( RENDERING_DEVICE_BASE& device, void* vertex_shader_blob ) = 0;
		void* m_resource = nullptr;
		std::vector<VERTEX_ATTRIBUTE> m_vertex_attributes;
	};
}
