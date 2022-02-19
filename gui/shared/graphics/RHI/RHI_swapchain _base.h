
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/math/vector2d.h"
#include "gui/shared/graphics/RHI/RHI_types.h"

namespace gui
{
	class SWAPCHAIN_BASE
	{
	public:
		inline static constexpr uint8_t s_swap_chain_buffer_count{ 3u };

		const core::FVECTOR2D& get_dimensions() const { return m_dimensions; }

		float get_width() const { return m_dimensions.x; }

		float get_height() const { return m_dimensions.y; }

		uint32_t get_buffer_count() const { return m_buffer_count; }

		uint32_t get_flags() const { return m_flags; }

		virtual bool resize( float width, float height, bool force = false ) = 0;

		virtual void present() = 0;

	protected:
		bool m_windowed{};
		uint32_t m_buffer_count{};
		uint32_t m_flags{};
		RHI_Format m_format = RHI_Format_R8G8B8A8_Unorm;
		core::FVECTOR2D m_dimensions;
	};
}
