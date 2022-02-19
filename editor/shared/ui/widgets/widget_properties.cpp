
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "editor/shared/ui/widgets/widget_properties.h"

#include "core/shared/math/transform.h"
#include "core/shared/patterns/scoped_value.h"
#include "editor/shared/events/editor_events.h"
#include "engine/shared/events/events_manager.h"
#include "engine/shared/game_framework/world_object.h"
#include "gui/shared/events/render_events.h"
#include "gui/shared/imgui/imgui.h"
#include "input/shared/input_events.h"

#include <limits>

namespace
{
	const auto show_vector = []( const char* label, core::FVECTOR3D& vector )
	{
		const auto show_float = []( const char* label, float* value )
		{
			constexpr float label_float_spacing = 15.0f;
			constexpr float step = 0.01f;
			const std::string format = "%.4f";

			// Label
			ImGui::TextUnformatted( label );
			ImGui::SameLine( label_float_spacing );

			// Float
			ImGui::PushItemWidth( 128.0f );
			ImGui::PushID( static_cast<int>(ImGui::GetCursorPosX() + ImGui::GetCursorPosY()) );
			ImGui::DragFloat( "##no_label", value, step, std::numeric_limits<float>::lowest(), 99999999.0f/*std::numeric_limits<float>::max()*/, format.c_str() );
			ImGui::PopID();
			ImGui::PopItemWidth();
		};

		constexpr float label_indetation = 15.0f;

		ImGui::BeginGroup();
		ImGui::Indent( label_indetation );
		ImGui::TextUnformatted( label );
		ImGui::Unindent( label_indetation );
		show_float( "X", &vector.x );
		show_float( "Y", &vector.y );
		show_float( "Z", &vector.z );
		ImGui::EndGroup();
	};

	const core::VECTOR<core::UNIQUE_ID> s_broadcastable_events
	{
		{
			editor::SELECT_WORLD_OBJECT_EVENT_ID
		}
	};
}

namespace editor
{
	WIDGET_PROPERTIES::WIDGET_PROPERTIES()
	{
		m_title = "Properties";

		m_max_size = { 1000.0f, 800.0f };
		m_bounds.set_dimensions( { 300.0f, 500.0f } );

		engine::EVENTS_MANAGER::get_singleton()->add_events_receiver( this, s_broadcastable_events );
	}

	WIDGET_PROPERTIES::~WIDGET_PROPERTIES()
	{
		engine::EVENTS_MANAGER::get_singleton()->remove_events_receiver( this );
	}

	bool WIDGET_PROPERTIES::receive_event( const engine::EVENT& in_event )
	{
		bool handled{ false };

		switch ( in_event.unique_id )
		{
		case engine::TICK_EVENT_ID:
			{
				handled = WIDGET::receive_event( in_event );
			}
			break;
		case SELECT_WORLD_OBJECT_EVENT_ID:
			{
				handled = WIDGET::receive_event( in_event );

				auto select_object_event{ static_cast<const SELECT_WORLD_OBJECT_EVENT&>(in_event) };

				m_selected_object = select_object_event.world_object;
			}
			break;
		case gui::MARK_FOR_RENDER_EVENT_ID:
			{
				handled = WIDGET::receive_event( in_event );
			}
			break;
		case gui::RENDER_IMGUI_EVENT_ID:
			{
				if ( m_should_render )
				{
					handled = WIDGET::receive_event( in_event );
				}
			}
			break;
		}

		return handled;
	}

	void WIDGET_PROPERTIES::render()
	{
		if ( m_is_visible && m_selected_object )
		{
			//core::SCOPED_VALUE scoped_value{ m_should_render, false };

			if ( ImGui::Begin( "Transform" ) )
			{
				auto& transform{ m_selected_object->get_transform() };
				auto position{ transform.get_position() };
				auto rotation{ transform.get_rotation().ToEulerAngles() };
				auto scale{ transform.get_scale() };

				show_vector( "Position", position );
				ImGui::SameLine();
				show_vector( "Rotation", rotation );
				ImGui::SameLine();
				show_vector( "Scale", scale );

				transform.set_position( position );
				transform.set_rotation( core::QUATERNION::FromEulerAngles( rotation ) );
				transform.set_scale( scale );

				ImGui::End();
			}
		}
	}
}
