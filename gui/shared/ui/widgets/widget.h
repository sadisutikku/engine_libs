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

#include "core/shared/math/rectangle.h"
#include "core/shared/math/vector2d.h"

#include <memory>
#include <optional>
#include <string>

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

struct ImGuiWindow;

namespace gui
{
	class WIDGET : public engine::NODE
	{
	public:
		WIDGET();
		virtual ~WIDGET() = default;
		
		bool receive_event( const engine::EVENT& in_event ) override;

		//virtual core::RECTANGLE<int32_t> calculate_desired_size() = 0; // todo make protected

		//virtual void arrange() = 0;

		//virtual void paint(graphics::RENDERER& renderer) = 0;

		bool set_size( const core::FVECTOR2D& possible_new_size )
		{
			if ( possible_new_size <= m_max_size )
			{
				m_bounds.set_dimensions( possible_new_size );
				return true;
			}
			else
			{
				return false;
			}
		}

	protected:
		virtual void render() = 0;

		bool m_is_visible{};
		bool m_should_render{};
		uint8_t m_var_pushes{};
		int32_t m_flags{};
		std::optional<float> m_alpha;
		core::FRECTANGLE m_bounds;
		core::FVECTOR2D m_position_override;
		core::FVECTOR2D m_size_override;
		core::FVECTOR2D m_padding;
		core::FVECTOR2D m_min_size;
		core::FVECTOR2D m_max_size;
		ImGuiWindow* m_window{};
		std::string m_title{ "Unnamed" };
	};
}
