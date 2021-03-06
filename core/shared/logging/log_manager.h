//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/automatic_singleton.h"

//////////////////////////////////////////////////////////////////////
//! Forward Declerations
//////////////////////////////////////////////////////////////////////

namespace core
{
	struct LOG_MESSAGE;
}

namespace core
{
	class LOG_MANAGER : public AUTOMATIC_SINGLETON<LOG_MANAGER>
	{
	public:
		void log_message(const core::LOG_MESSAGE& in_log_message);
	};
}
