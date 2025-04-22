#pragma once

#include "UltraUtilities/Defines.h"

namespace UU
{
	/**
	 * These are dynamic arrays.
	 */
	template<typename V>
	class UU_API DArray
	{
	public:
		DArray(unsigned int size = 0)
		{
			this->bufferSize = (size == 0) ? 1 : size;
			this->buffer = new V[this->bufferSize];
			this->arraySize = size;
		}

		DArray(const DArray& array)
		{
			this->bufferSize = array.bufferSize;
			this->arraySize = array.arraySize;
			this->buffer = new V[array.bufferSize];
			for (unsigned int i = 0; i < array.arraySize; i++)
				this->buffer[i] = array.buffer[i];
		}

		DArray(DArray&& array)
		{
			this->bufferSize = array.bufferSize;
			this->arraySize = array.arraySize;
			this->buffer = array.buffer;
			array.buffer = nullptr;
		}

		DArray(const V* givenBuffer, unsigned int givenBufferSize)
		{
			this->bufferSize = givenBufferSize;
			this->arraySize = givenBufferSize;
			this->buffer = new V[givenBufferSize];
			for (unsigned int i = 0; i < givenBufferSize; i++)
				this->buffer[i] = givenBuffer[i];
		}

		virtual ~DArray()
		{
			delete[] this->buffer;
		}

		void operator=(const DArray& array)
		{
			this->EnsureCapacity(array.arraySize);
			this->SetSize(array.arraySize);
			for (unsigned int i = 0; i < array.arraySize; i++)
				this->buffer[i] = array.buffer[i];
		}

		void operator=(DArray&& array)
		{
			this->bufferSize = array.bufferSize;
			this->arraySize = array.arraySize;
			this->buffer = array.buffer;
			array.buffer = nullptr;
		}

		V& operator[](unsigned int i)
		{
			return this->buffer[i % this->arraySize];
		}

		const V& operator[](unsigned int i) const
		{
			return this->buffer[i % this->arraySize];
		}

		void Push(V value)
		{
			if (this->arraySize == this->bufferSize)
				this->SetCapacity(this->bufferSize * 2);

			this->buffer[this->arraySize++] = value;
		}

		bool Pop(V* value = nullptr)
		{
			if (this->arraySize == 0)
				return false;
			if (value)
				*value = this->buffer[this->arraySize - 1];
			this->arraySize--;
			return true;
		}

		V Pop(V defaultValue)
		{
			V value = defaultValue;
			this->Pop(&value);
			return value;
		}

		V* GetBuffer()
		{
			return this->buffer;
		}

		const V* GetBuffer() const
		{
			return this->buffer;
		}

		unsigned int GetSize() const
		{
			return this->arraySize;
		}

		void SetSize(unsigned int size)
		{
			if (size > this->bufferSize)
				this->SetCapacity(size);
			this->arraySize = size;
		}

		unsigned int GetCapacity() const
		{
			return this->bufferSize;
		}

		bool SetCapacity(unsigned int capacity)
		{
			if (capacity < this->arraySize)
				return false;
			V* newBuffer = new V[capacity];
			for (unsigned int i = 0; i < this->arraySize; i++)
				newBuffer[i] = this->buffer[i];
			delete[] this->buffer;
			this->buffer = newBuffer;
			this->bufferSize = capacity;
			return true;
		}

		void EnsureCapacity(unsigned int capacity)
		{
			if (this->bufferSize < capacity)
				this->SetCapacity(capacity);
		}

	private:
		V* buffer;
		unsigned int bufferSize;
		unsigned int arraySize;
	};
}