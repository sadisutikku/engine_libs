
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "core/shared/debugging/asserts.h"

#include <memory>

namespace core
{
	template<typename TYPE>
	class NON_AUTOMATIC_SINGLETON
	{
	public:
		static bool is_availabile()
		{
			return m_singleton != nullptr;
		}

		static TYPE* get_singleton()
		{
			ASSERT_TRUE( m_singleton );
			return m_singleton.get();
		}

		template<typename... ARGS>
		static TYPE* create_singleton( ARGS&&... in_args )
		{
			if ( m_singleton )
			{
				ASSERT_SHOULDNT_BE_HERE();
				return nullptr;
			}

			m_singleton = std::unique_ptr<TYPE>{ TYPE::create_singleton_custom( std::forward<ARGS>( in_args )... ) };
			return m_singleton.get();
		}

		static TYPE* set_singleton_with_child( TYPE* in_object ) // eg usage EDITOR_ENGINE::set_singleton(new EDITOR_ENGINE);
		{
			ASSERT_FALSE( m_singleton );
			m_singleton = in_object;
			return m_singleton;
		}

		static void destroy_singleton()
		{
			m_singleton.reset();
		}

	protected:
		NON_AUTOMATIC_SINGLETON() = default;
		virtual ~NON_AUTOMATIC_SINGLETON() = default;

		static inline std::unique_ptr<TYPE> m_singleton;

		template<typename... ARGS>
		static TYPE* create_singleton_custom( ARGS&&... in_args )
		{
			return new TYPE( std::forward<ARGS>( in_args )... );
		}

	private:
		NON_AUTOMATIC_SINGLETON( const NON_AUTOMATIC_SINGLETON& ) = delete;
		NON_AUTOMATIC_SINGLETON( const NON_AUTOMATIC_SINGLETON&& ) = delete;
		NON_AUTOMATIC_SINGLETON& operator= ( const NON_AUTOMATIC_SINGLETON& ) = delete;
		NON_AUTOMATIC_SINGLETON& operator= ( const NON_AUTOMATIC_SINGLETON&& ) = delete;
	};
}