

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/quad_d3d11.h"

#include "core/shared/debugging/asserts.h"
#include "core/shared/type_wrappers/array.h"
#include "core/shared/vertex.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/buffer_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"

namespace
{
	constexpr core::ARRAY<uint32_t, gui::NUM_VERTICES> indices{ 0, 1, 2, 3, 4, 5 };
}

namespace gui
{
	QUAD_D3D11::QUAD_D3D11( RENDERING_DEVICE_D3D11& device, const core::FVECTOR2D& swapchain_dimensions )
		: core::FRECTANGLE{ 0.0f, 0.0f, swapchain_dimensions.x, swapchain_dimensions.y }
		, m_vertex_buffer{ std::make_unique<BUFFER_D3D11>() }
		, m_index_buffer{ std::make_unique<BUFFER_D3D11>() }
	{
		const auto sc_left = -(swapchain_dimensions.x * 0.5f) + get_left();
		const auto sc_right = sc_left + get_width();
		const auto sc_top = (swapchain_dimensions.y * 0.5f) - get_top();
		const auto sc_bottom = sc_top - get_height();

		/*const auto sc_left = -0.5f;
		const auto sc_right = 0.5f;
		const auto sc_top = 0.5f;
		const auto sc_bottom = -0.5f;*/

		// Create vertex buffer
		const core::ARRAY<core::RHI_Vertex_PosTex, NUM_VERTICES> vertices
		{
			// First triangle
			core::RHI_Vertex_PosTex( { { sc_left,    sc_top,        0.0f },    {  0.0f, 0.0f } } ),    // Top left    
			core::RHI_Vertex_PosTex( { { sc_right,    sc_bottom,    0.0f },    { 1.0f, 1.0f } } ),    // Bottom right    
			core::RHI_Vertex_PosTex( { { sc_left,    sc_bottom,    0.0f },    { 0.0f, 1.0f } } ),    // Bottom left
			// Second triangle
			core::RHI_Vertex_PosTex( { { sc_left,    sc_top,        0.0f },    { 0.0f, 0.0f } } ),    // Top left    
			core::RHI_Vertex_PosTex( { { sc_right,    sc_top,        0.0f },    { 1.0f, 0.0f } } ),    // Top right        
			core::RHI_Vertex_PosTex( { { sc_right,    sc_bottom,    0.0f },    { 1.0f, 1.0f } } )    // Bottom right
		};

		if ( !m_vertex_buffer->create( device, BUFFER_TYPE::VERTEX, vertices ) )
		{
			ASSERT_FAILED( "Failed to create vertex buffer." );
		}

		if ( !m_index_buffer->create( device, BUFFER_TYPE::VERTEX, indices ) )
		{
			ASSERT_FAILED( "Failed to create index buffer." );;
		}
	}

	QUAD_D3D11::~QUAD_D3D11() = default;
}
