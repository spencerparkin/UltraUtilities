#pragma once

#include "UltraUtilities/Defines.h"

namespace UU
{
	/**
	 * This defines an interface to an object free-store.
	 */
	template<typename T>
	class UU_API ObjectHeap
	{
	public:
		virtual T* Allocate() = 0;
		virtual bool Deallocate(T* object) = 0;
	};

	/**
	 * The default implementation just uses the built-in free-store.
	 */
	template<typename T>
	class UU_API DefaultObjectHeap : public ObjectHeap<T>
	{
	public:
		virtual T* Allocate() override
		{
			return new T();
		}

		virtual bool Deallocate(T* object) override
		{
			delete object;
			return true;
		}
	};

	/**
	 * Object pools have O(1) time allocation and deallocation, but
	 * there is a pre-determined limit to the number of allocations that
	 * can be made from the heap.  (Well, there always is such a limit,
	 * but here the bound can't practically be the memory limit of the
	 * machine.)
	 */
	template<typename T>
	class UU_API ObjectPoolHeap : public ObjectHeap<T>
	{
	public:
		ObjectPoolHeap(unsigned int maxObjects)
		{
			this->freeObjectStackSize = maxObjects;
			this->freeObjectStack = new unsigned int[maxObjects];
			this->freeObjectStackTop = 0;
			for (unsigned int i = 0; i < maxObjects; i++)
				this->freeObjectStack[i] = i * sizeof(T);
			this->buffer = new unsigned char[maxObjects * sizeof(T)];
		}

		virtual ~ObjectPoolHeap()
		{
			delete[] this->freeObjectStack;
			delete[] this->buffer;
		}

		virtual T* Allocate() override
		{
			if (this->freeObjectStackTop == this->freeObjectStackSize)
				return nullptr;
			unsigned int i = this->freeObjectStack[this->freeObjectStackTop++];
			unsigned char* memory = &this->buffer[i];
			T* object = new (memory) T();
			return object;
		}

		virtual bool Deallocate(T* object) override
		{
			if (this->freeObjectStackTop == 0)
				return false;
			auto memory = static_cast<unsigned char*>(object);
			unsigned int i = memory - this->buffer;
			UU_ASSERT(i % sizeof(T) == 0 && i / sizeof(T) < this->freeObjectStackSize);
			if (i % sizeof(T) != 0 || i / sizeof(T) >= this->freeObjectStackSize)
				return false;
			this->freeObjectStack[--this->freeObjectStackTop] = i;
			object->~T();
			return true;
		}

	private:
		unsigned char* buffer;
		unsigned int* freeObjectStack;
		unsigned int freeObjectStackSize;
		unsigned int freeObjectStackTop;
	};
}