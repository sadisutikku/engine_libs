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

#include "core/shared/math/transform.h"

#include <memory>
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
    class MESH;

    class WORLD_OBJECT
    {
    public:
        WORLD_OBJECT();
        ~WORLD_OBJECT();
        WORLD_OBJECT( const engine::WORLD_OBJECT& other );

        void create_mesh( std::vector<core::RHI_Vertex_PosTexNorTan>&& vertices, std::vector<uint32_t>&& indices, gui::MATERIAL* material );

        MESH* get_mesh() const { return m_mesh.get(); }

        void move( const core::FVECTOR3D& movement );
        void rotate( const core::QUATERNION& rotation );
        void scale( const core::FVECTOR3D& scale );

        const core::TRANSFORM& get_transform() const { return m_transform; }
        core::TRANSFORM& get_transform() { return m_transform; }

    private:
        core::TRANSFORM m_transform;
        std::unique_ptr<MESH> m_mesh;
    };
}