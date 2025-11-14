#pragma once

#include "UltraUtilities/Defines.h"

namespace UU
{
	template<typename V> class DArrayIterator;

	/**
	 * These are dynamic arrays.
	 */
	template<typename V>
	class UU_API DArray
	{
	public:
		DArray(unsigned int size = 0)
		{
			this->iterationDirection = DArrayIterator<V>::Direction::FORWARD;
			this->bufferSize = (size == 0) ? 1 : size;
			this->buffer = new V[this->bufferSize];
			this->arraySize = size;
		}

		DArray(const DArray& array)
		{
			this->iterationDirection = DArrayIterator<V>::Direction::FORWARD;
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

		/**
		 * Append the given value to the end of this array.
		 */
		void Push(V value)
		{
			if (this->arraySize == this->bufferSize)
				this->SetCapacity(this->bufferSize * 2);

			this->buffer[this->arraySize++] = value;
		}

		/**
		 * Remove the value at the end of this array.
		 */
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

		/**
		 * Return the number of elements in this array.
		 */
		unsigned int GetSize() const
		{
			return this->arraySize;
		}

		/**
		 * Set the number of elements in this array.  If the new size is smaller,
		 * elements are lost.  If the new size is larger, then uninitialized
		 * elements are added to the array.
		 */
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
			// STPTODO: A realloc here would be much more efficient if possible.
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

		/**
		 * Find the position of the given value in this array.
		 */
		unsigned int Find(const V& value)
		{
			for (unsigned int i = 0; i < this->arraySize; i++)
				if (this->buffer[i] == value)
					return i;

			return -1;
		}

		/**
		 * Remove the value at the given position from this array while not preserving array order.
		 */
		bool QuickRemove(unsigned int i)
		{
			if (i >= this->arraySize)
				return false;

			if (i != this->arraySize - 1)
				this->buffer[i] = this->buffer[this->arraySize - 1];

			this->arraySize--;
			return true;
		}

		/**
		 * Remove the value at the given position from this array while preserving array order.
		 */
		bool ShiftRemove(unsigned int i)
		{
			if (i >= this->arraySize)
				return false;

			if (i != this->arraySize - 1)
				for (unsigned int j = i; j < this->arraySize - 1; j++)
					this->buffer[j] = this->buffer[j + 1];

			this->arraySize--;
			return true;
		}

		/**
		 * Find and remove the given value from this array.
		 */
		bool Remove(const V& value)
		{
			unsigned int i = this->Find(value);
			if (i == -1)
				return false;
			return this->ShiftRemove(i);
		}

		/**
		 * Quickly insert the given value at the given position while not preserving array order.
		 */
		bool QuickInsert(unsigned int i, V value)
		{
			if (i > this->arraySize)
				return false;

			if (this->arraySize == this->bufferSize)
				this->SetCapacity(this->bufferSize * 2);

			this->arraySize++;

			if (this->arraySize - 1 != i)
				this->buffer[this->arraySize - 1] = this->buffer[i];

			this->buffer[i] = value;
			return true;
		}

		/**
		 * Insert the given value at the given position while preserving array order.
		 */
		bool ShiftInsert(unsigned int i, V value)
		{
			if (i > this->arraySize)
				return false;

			if (this->arraySize == this->bufferSize)
				this->SetCapacity(this->bufferSize * 2);

			this->arraySize++;
			for (unsigned int j = this->arraySize - 1; j > i; j--)
				this->buffer[j] = this->buffer[j - 1];

			this->buffer[i] = value;
			return true;
		}

		DArrayIterator<V> begin()
		{
			return DArrayIterator<V>(this, this->iterationDirection);
		}

		int end()
		{
			switch (this->iterationDirection)
			{
			case DArrayIterator<V>::Direction::FORWARD:
				return this->arraySize;
			case DArrayIterator<V>::Direction::BACKWARD:
				return -1;
			}

			return 0;
		}

		/**
		 * This is a super lame and slow bubble sort.  It is a stable sort.
		 * I think the worst-case running time may be when the list is in reverse order.
		 * The best-case running time, though, is linear when the list is already sorted.
		 */
		template<typename P>
		void Sort(P predicate)
		{
			if (this->arraySize == 0)
				return;
			bool sorted = false;
			while (!sorted)
			{
				sorted = true;
				for (unsigned int i = 0; i < this->arraySize - 1; i++)
				{
					V& valueA = this->buffer[i];
					V& valueB = this->buffer[i + 1];
					int compare = predicate(valueA, valueB);
					if (compare > 0)
					{
						V valueC = valueA;
						valueA = valueB;
						valueB = valueC;
						sorted = false;
					}
				}
			}
		}

	private:
		V* buffer;
		unsigned int bufferSize;
		unsigned int arraySize;
		mutable DArrayIterator<V>::Direction iterationDirection;
	};

	/**
	 * This is used to make the @ref DArray class compatible with the ranged for-loop syntax.
	 */
	template<typename V>
	class DArrayIterator
	{
	public:
		enum Direction
		{
			FORWARD,
			BACKWARD
		};

		DArrayIterator(DArray<V>* array, Direction direction)
		{
			this->array = array;
			this->direction = direction;

			switch (direction)
			{
			case FORWARD:
				this->i = 0;
				break;
			case BACKWARD:
				this->i = int(array->GetSize()) - 1;
				break;
			}
		}

		void operator++()
		{
			switch (this->direction)
			{
			case FORWARD:
				this->i++;
				break;
			case BACKWARD:
				this->i--;
				break;
			}
		}

		bool operator==(int j)
		{
			return this->i == j;
		}

		V& operator*()
		{
			return (*this->array)[this->i];
		}

	private:
		DArray<V>* array;
		Direction direction;
		int i;
	};
}