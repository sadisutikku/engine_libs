//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

namespace core
{
	// Sets a temp value, the returns the variable to it's original value we going out-of-scope

	template<typename TYPE>
	class INTERIM_VALUE
	{
	public:
		INTERIM_VALUE(TYPE& in_variable, TYPE in_new_value)
			: m_variable(in_variable)
			, m_previous_value(in_variable)
		{
			m_variable = in_new_value;
		}

		~INTERIM_VALUE()
		{
			m_variable = m_previous_value;
		}

	private:
		TYPE& m_variable;

		TYPE m_previous_value;
	};


	// Sets the variable to a certain value when going out-of-scope

	template<typename TYPE>
	class SCOPED_VALUE
	{
	public:
		SCOPED_VALUE( TYPE& in_variable, TYPE in_future_value )
			: m_variable( in_variable )
		{
			m_future_value = in_future_value;
		}

		~SCOPED_VALUE()
		{
			m_variable = m_future_value;
		}

	private:
		TYPE& m_variable;

		TYPE m_future_value;
	};
}
