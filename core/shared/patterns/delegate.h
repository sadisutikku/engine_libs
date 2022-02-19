//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <functional>
#include <mutex> 
#include <unordered_map>

namespace core
{
	template<typename TYPE>
	struct DELEGATE_INNER
	{
		void set_function(const TYPE& in_function)
		{
			m_function = in_function;
		}

		template<typename RETURN_TYPE, typename... ARGS>
		bool execute( ARGS&&... in_args )
		{
			if ( m_function )
			{
				m_function( std::forward<ARGS>(in_args)... );
				return true;
			}

			return false;
		}

	private:
		TYPE m_function;
	};

	template<typename TYPE>
	struct DELEGATE
	{
		std::function<TYPE()> function;
	};

	template<>
	struct DELEGATE<void>
	{
		std::function<void()> function;
	};

	template<typename RETURN_TYPE, typename TYPE>
	class DELEGATE_ONE_PARAM
	{
		using DELEGATE_TYPE = std::function<RETURN_TYPE(TYPE)>;
		using INNER_TYPE = DELEGATE_INNER<DELEGATE_TYPE>;

	public:
		INNER_TYPE& operator->()
		{
			return m_inner;
		}

	//private:
		INNER_TYPE m_inner;
	};

	template<typename TYPE>
	class DELEGATE_ONE_PARAM<void, TYPE>
	{
		using DELEGATE_TYPE = std::function<void(TYPE)>;
		using INNER_TYPE = DELEGATE_INNER<DELEGATE_TYPE>;

	public:
		INNER_TYPE& operator->()
		{
			return m_inner;
		}

	//private:
		INNER_TYPE m_inner;
	};

	template<typename TYPE>
	struct MULTICAST_DELEGATE_INNER
	{
		using ID_TYPE = size_t;

		ID_TYPE add_function(TYPE in_function)
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			m_function_array.insert( { CURRENT_ID++, std::move(in_function) } );
			return CURRENT_ID;
		}

		void remove_function(ID_TYPE in_id)
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			m_function_array.erase(in_id);
		}

		template<typename RETURN_TYPE, typename... ARGS>
		bool execute( ARGS&&... in_args )
		{
			std::lock_guard<std::mutex> guard(m_mutex);

			for (auto const& [id, function] : m_function_array )
			{
				function( std::forward<ARGS>(in_args)... );
			}

			return m_function_array.size() > 0;
		}

	private:
		static inline ID_TYPE INVALID_ID{ 0 };
		static inline ID_TYPE CURRENT_ID{ 1 }; //!< Invalid id is 0, so start at 1
		std::unordered_map<ID_TYPE, TYPE> m_function_array;

		std::mutex m_mutex;
	};

	template<typename RETURN_TYPE>
	class MULTICAST_DELEGATE
	{
		using DELEGATE_TYPE = std::function<RETURN_TYPE()>;

		MULTICAST_DELEGATE_INNER<DELEGATE_TYPE> m_inner;
	};

	template<>
	class MULTICAST_DELEGATE<void>
	{
		using DELEGATE_TYPE = std::function<void()>;

		MULTICAST_DELEGATE_INNER<DELEGATE_TYPE> m_inner;
	};

	template<typename RETURN_TYPE, typename TYPE>
	class MULTICAST_DELEGATE_ONE_PARAM
	{
		using DELEGATE_TYPE = std::function<RETURN_TYPE(TYPE)>;
		using INNER_TYPE = MULTICAST_DELEGATE_INNER<DELEGATE_TYPE>;

	public:
		INNER_TYPE& operator->()
		{
			return m_inner;
		}

	//private:
		INNER_TYPE m_inner;
	};

	template<typename TYPE>
	class MULTICAST_DELEGATE_ONE_PARAM<void, TYPE>
	{
		using DELEGATE_TYPE = std::function<void(TYPE)>;
		using INNER_TYPE = MULTICAST_DELEGATE_INNER<DELEGATE_TYPE>;

	public:
		INNER_TYPE& operator->()
		{
			return m_inner;
		}

	//private:
		INNER_TYPE m_inner;
	};

	template<typename RETURN_TYPE, typename TYPE1, typename TYPE2>
	class MULTICAST_DELEGATE_TWO_PARAMS
	{
		using DELEGATE_TYPE = std::function<RETURN_TYPE(TYPE1, TYPE2)>;
		using INNER_TYPE = MULTICAST_DELEGATE_INNER<DELEGATE_TYPE>;

	public:
		INNER_TYPE& operator->() const
		{
			return m_inner;
		}

		//private:
		INNER_TYPE m_inner;
	};

	template<typename TYPE1, typename TYPE2>
	class MULTICAST_DELEGATE_TWO_PARAMS<void, TYPE1, TYPE2>
	{
		using DELEGATE_TYPE = std::function<void(TYPE1, TYPE2)>;
		using INNER_TYPE = MULTICAST_DELEGATE_INNER<DELEGATE_TYPE>;

	public:
		INNER_TYPE& operator->()
		{
			return m_inner;
		}

		//private:
		INNER_TYPE m_inner;
	};
}
