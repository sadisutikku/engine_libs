
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/ui/widgets/widget.h"

#include "gui/shared/screen/camera3d.h"

#include <memory>

//////////////////////////////////////////////////////////////////////
//! Forward Declarations
//////////////////////////////////////////////////////////////////////

namespace engine
{
	class WORLD;
}

namespace editor
{
	class WIDGET_VIEWPORT : public gui::WIDGET
	{
	public:
		WIDGET_VIEWPORT();
		~WIDGET_VIEWPORT() = default;

		bool receive_event( const engine::EVENT& in_event ) override;

	private:
		void render() override;

		bool m_allow_camera_movement{};
		engine::WORLD* m_world{};
		gui::CAMERA3D m_camera3d;
	};
}
