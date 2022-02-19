
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/rectangle.h"

#include "core/shared/math/vector_types.h"

#include <memory>

//////////////////////////////////////////////////////////////////////
//! Constants
//////////////////////////////////////////////////////////////////////

namespace gui
{
	inline constexpr uint32_t NUM_VERTICES{ 6u };
}

namespace gui
{
	class BUFFER_D3D11;
	class RENDERING_DEVICE_D3D11;

	struct QUAD_D3D11 : public core::FRECTANGLE
	{
	public:
		QUAD_D3D11( RENDERING_DEVICE_D3D11& device, const core::FVECTOR2D& swapchain_dimensions );
		~QUAD_D3D11();

		BUFFER_D3D11* get_vertex_buffer() const { return m_vertex_buffer.get(); }

		BUFFER_D3D11* get_index_buffer() const { return m_index_buffer.get(); }

		static constexpr uint32_t get_num_indices() { return NUM_VERTICES; }

	private:
		std::unique_ptr<BUFFER_D3D11> m_vertex_buffer;
		std::unique_ptr<BUFFER_D3D11> m_index_buffer;
	};
}
