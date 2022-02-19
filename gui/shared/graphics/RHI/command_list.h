
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/command_list_base.h"

#include <memory>

namespace gui
{
	enum class RHI_Api_Type : uint8_t;

	class COMMAND_LIST
	{
	public:
		COMMAND_LIST( RHI_Api_Type api_type );
		~COMMAND_LIST();

	private:
		std::unique_ptr<COMMAND_LIST_BASE> m_command_list;
	};
}
