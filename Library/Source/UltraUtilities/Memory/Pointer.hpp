#pragma once

#include "UltraUtilities/Defines.h"

namespace UU
{
	/**
	 * 
	 */
	template<typename T>
	class UU_API UniquePtr
	{
	public:
		UniquePtr(T* givenPointer)
		{
			this->pointer = givenPointer;
		}

		virtual ~UniquePtr()
		{
			delete this->pointer;
		}

		void Reset(T* givenPointer = nullptr)
		{
			this->pointer = givenPointer;
		}

		T* Release()
		{
			T* resultPointer = this->pointer;
			this->pointer = nullptr;
			return resultPointer;
		}

		T* operator->()
		{
			return this->pointer;
		}

		const T* operator->() const
		{
			return this->pointer;
		}

		T* Get()
		{
			return this->pointer;
		}

		const T* Get() const
		{
			return this->pointer;
		}

		operator T* ()
		{
			return this->pointer;
		}

		operator const T* () const
		{
			return this->pointer;
		}

	private:
		T* pointer;
	};

	// STPTODO: Add shared and weak pointer implementations here.
}