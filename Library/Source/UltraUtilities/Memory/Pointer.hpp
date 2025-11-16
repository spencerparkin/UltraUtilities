#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/HashMap.hpp"

namespace UU
{
	/**
	 * 
	 */
	template<typename T>
	class UU_API UniquePtr
	{
	public:
		UniquePtr(T* givenPointer = nullptr)
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

	/**
	 * This class is used internally and is not meant to be instantiated by the user.
	 */
	class UU_API Reference
	{
	public:
		Reference(void* pointer);
		virtual ~Reference();

	public:
		static unsigned long long nextHandle;
		static HashMap<unsigned long long, Reference*> refMap;

		void* pointer;
		unsigned int count;
		unsigned long long handle;
	};

	/**
	 * This is my attempt to make a shared pointer.  It's probably dumb.
	 */
	template<typename T>
	class UU_API SharedPtr
	{
		template<typename T>
		friend class WeakPtr;

		template<typename>
		friend class SharedPtr;

	private:
		SharedPtr(Reference* ref)
		{
			this->ref = ref;
		}

	public:
		SharedPtr()
		{
			this->ref = nullptr;
		}

		SharedPtr(SharedPtr<T>& sharedPtr)
		{
			this->ref = sharedPtr.ref;
			this->ref->count++;
		}

		SharedPtr(const SharedPtr<T>& sharedPtr)
		{
			this->ref = sharedPtr.ref;
			this->ref->count++;
		}

		SharedPtr(SharedPtr<T>&& sharedPtr)
		{
			this->ref = sharedPtr.ref;
			sharedPtr.ref = nullptr;
		}

		template<typename T2>
		SharedPtr(SharedPtr<T2>& sharedPtr)
		{
			this->ref = nullptr;
			if (sharedPtr.ref)
			{
				const T* pointer = sharedPtr.Get();	// This line is only here to make sure that the cast is valid at compile-time.
				this->ref = sharedPtr.ref;
				this->ref->count++;
			}
		}

		template<typename T2>
		SharedPtr(const SharedPtr<T2>& sharedPtr)
		{
			this->ref = nullptr;
			if (sharedPtr.ref)
			{
				const T* pointer = sharedPtr.Get();	// This line is only here to make sure that the cast is valid at compile-time.
				this->ref = sharedPtr.ref;
				this->ref->count++;
			}
		}

		virtual ~SharedPtr()
		{
			if (this->ref && --this->ref->count == 0)
				delete this->ref;
		}

		template<typename T2>
		void operator=(SharedPtr<T2>& sharedPtr)
		{
			this->ref = nullptr;
			if (sharedPtr.ref)
			{
				const T* pointer = sharedPtr.Get();	// This line is only here to make sure that the cast is valid at compile-time.
				this->ref = sharedPtr.ref;
				this->ref->count++;
			}
		}

		template<typename T2>
		void operator=(const SharedPtr<T2>& sharedPtr)
		{
			this->ref = nullptr;
			if (sharedPtr.ref)
			{
				const T* pointer = sharedPtr.Get();	// This line is only here to make sure that the cast is valid at compile-time.
				this->ref = sharedPtr.ref;
				this->ref->count++;
			}
		}

		static SharedPtr<T> Make()
		{
			Reference* ref = new Reference(new T());
			return SharedPtr<T>(ref);
		}

		template<class... Args>
		static SharedPtr<T> Make(Args&&... args)
		{
			Reference* ref = new Reference(new T(args...));
			return SharedPtr<T>(ref);
		}

		T* Get()
		{
			return this->ref ? static_cast<T*>(this->ref->pointer) : nullptr;
		}

		const T* Get() const
		{
			return this->ref ? static_cast<const T*>(this->ref->pointer) : nullptr;
		}

	private:
		Reference* ref;
	};

	/**
	 * This is my attempt to make a weak pointer.  It's probably dumb.
	 */
	template<typename T>
	class UU_API WeakPtr
	{
	public:
		WeakPtr()
		{
			this->handle = 0;
		}

		WeakPtr(WeakPtr<T>& weakPtr)
		{
			this->handle = weakPtr.handle;
		}

		WeakPtr(SharedPtr<T>& sharedPtr)
		{
			this->handle = sharedPtr.ref ? sharedPtr.ref->handle : 0;
		}

		virtual ~WeakPtr()
		{
		}

		SharedPtr<T> Get()
		{
			Reference* ref = nullptr;
			Reference::refMap.Find(this->handle, &ref);
			ref->count++;
			return SharedPtr<T>(ref);
		}

	private:
		unsigned long long handle;
	};
}