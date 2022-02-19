//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/patterns/delegate.h"

#include <unordered_set>

namespace core
{
	template<typename TYPE>
	class INSTANCE_TRACKER
	{
	public:
		static void add_instance(TYPE* in_object)
		{
			m_instances.insert(in_object);

			m_on_added_delegate.m_inner.execute<void, TYPE*>( std::move( in_object ) );
		}

		static void remove_instance(TYPE* in_object)
		{
			auto itr = std::find(m_instances.begin(), m_instances.end(), in_object);
			if (itr != m_instances.end())
			{
				m_pre_removed_delegate.m_inner.execute<void, TYPE*>( std::move( in_object ) );

				m_instances.erase(itr);
			}
		}

		static const std::unordered_set<TYPE*>& get_instances()
		{
			return m_instances;
		}

		static size_t get_number_of_instances()
		{
			return m_instances.size();
		}

		static inline MULTICAST_DELEGATE_ONE_PARAM<void, TYPE*> m_on_added_delegate;
		static inline MULTICAST_DELEGATE_ONE_PARAM<void, TYPE*> m_pre_removed_delegate;

	private:
		INSTANCE_TRACKER() = default;
		~INSTANCE_TRACKER() = default;

		static inline std::unordered_set<TYPE*> m_instances;
	};
}
