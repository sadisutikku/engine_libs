//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "engine/shared/game_framework/world_object.h"

#include "engine/shared/game_framework/mesh.h"

namespace engine
{
    WORLD_OBJECT::WORLD_OBJECT() = default;

    WORLD_OBJECT::~WORLD_OBJECT() = default;

    WORLD_OBJECT::WORLD_OBJECT( const engine::WORLD_OBJECT& ) {}

    void WORLD_OBJECT::create_mesh( std::vector<core::RHI_Vertex_PosTexNorTan>&& vertices, std::vector<uint32_t>&& indices, gui::MATERIAL* material )
    {
        m_mesh = std::make_unique<MESH>( std::move( vertices ), std::move( indices ), material );
    }

    void WORLD_OBJECT::move( const core::FVECTOR3D& movement )
    {
        m_transform.move( movement );
    }

    void WORLD_OBJECT::rotate( const core::QUATERNION& rotation )
    {
        m_transform.rotate( rotation );
    }

    void WORLD_OBJECT::scale( const core::FVECTOR3D& scale )
    {
        m_transform.scale( scale );
    }
}