
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/non_automatic_singleton.h"

namespace core
{
	template<typename TYPE>
	class AUTOMATIC_SINGLETON : public NON_AUTOMATIC_SINGLETON<TYPE>
	{
		using super = NON_AUTOMATIC_SINGLETON<TYPE>;

	public:
		static TYPE& get_singleton()
		{
			if ( !super::is_availabile() )
			{
				super::create_singleton();
			}

			return *super::get_singleton();
		}
	};
}