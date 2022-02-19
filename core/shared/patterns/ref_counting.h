//////////////////////////////////////////////////////////////////////
//! MIT License
//! Copyright (c) 2022 sadisutikku
//! Part of https://github.com/sadisutikku/engine_libs.git
//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <atomic>

namespace core
{
	template<typename TYPE>
	class SHARED_POINTER
	{
	public:
		virtual void add_reference()
		{
			++m_ref_count;
		}

		virtual void release_reference()
		{
			--m_ref_count;
			if (m_ref_count < 1)
			{
				delete m_reference;
				m_reference = nullptr;
			}
		}

		SHARED_POINTER()
			: m_reference(nullptr)
		{}

		SHARED_POINTER(TYPE* in_reference)
		{
			m_reference = in_reference;
			if (m_reference)
			{
				m_reference->add_reference();
			}
		}

		SHARED_POINTER(const SHARED_POINTER& in_copy)
		{
			m_reference = in_copy.m_reference;
			if (m_reference)
			{
				m_reference->add_reference();
			}
		}

		SHARED_POINTER(SHARED_POINTER&& in_copy)
		{
			m_reference = Copy.m_reference;
			in_copy.m_reference = nullptr;
		}

		~SHARED_POINTER()
		{
			if (m_reference)
			{
				m_reference->release_reference();
			}
		}

		SHARED_POINTER& operator=(TYPE* in_reference)
		{
			// Call add_reference before Release, in case the new reference is the same as the old reference.
			TYPE* OldReference = m_reference;
			m_reference = in_reference;
			if (m_reference)
			{
				m_reference->add_reference();
			}
			if (OldReference)
			{
				OldReference->release_reference();
			}
			return *this;
		}

		SHARED_POINTER& operator=(const SHARED_POINTER& InPtr)
		{
			return *this = InPtr.m_reference;
		}

		SHARED_POINTER& operator=(SHARED_POINTER&& InPtr)
		{
			if (this != &InPtr)
			{
				TYPE* OldReference = m_reference;
				m_reference = InPtr.m_reference;
				InPtr.m_reference = nullptr;
				if (OldReference)
				{
					OldReference->release_reference();
				}
			}
			return *this;
		}

		TYPE* operator->() const
		{
			return m_reference;
		}

		using ReferenceType = TYPE*;
		operator ReferenceType() const
		{
			return m_reference;
		}

		friend uint32_t GetTypeHash(const SHARED_POINTER& InPtr)
		{
			return GetTypeHash(InPtr.m_reference);
		}

		TYPE** GetInitReference()
		{
			*this = nullptr;
			return &m_reference;
		}

		TYPE* GetReference() const
		{
			return m_reference;
		}

		friend bool IsValidRef(const SHARED_POINTER& in_reference)
		{
			return in_reference.m_reference != nullptr;
		}

		bool IsValid() const
		{
			return m_reference != nullptr;
		}

		void SafeRelease()
		{
			*this = nullptr;
		}

		uint32_t GetRefCount()
		{
			uint32 Result = 0;
			if (m_reference)
			{
				Result = m_reference->GetRefCount();
				check(Result > 0); // you should never have a zero ref count if there is a live ref counted pointer (*this is live)
			}
			return Result;
		}

		void Swap(SHARED_POINTER& InPtr) // this does not change the reference count, and so is faster
		{
			TYPE* OldReference = m_reference;
			m_reference = InPtr.m_reference;
			InPtr.m_reference = OldReference;
		}

	private:
		TYPE* m_reference{ nullptr };

		std::atomic<uint32_t> m_ref_count{ 0 };
	};

	template<typename TYPE>
	bool operator==(const SHARED_POINTER<TYPE>& A, const SHARED_POINTER<TYPE>& B)
	{
		return A.GetReference() == B.GetReference();
	}

	template<typename TYPE>
	bool operator==(const SHARED_POINTER<TYPE>& A, TYPE* B)
	{
		return A.GetReference() == B;
	}

	template<typename TYPE>
	bool operator==(TYPE* A, const SHARED_POINTER<TYPE>& B)
	{
		return A == B.GetReference();
	}
}
