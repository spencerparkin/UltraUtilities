#pragma once

#include "UltraUtilities/Containers/DArray.hpp"

namespace UU
{
	/**
	 * This class abstracts the notion of comparing keys in a priority queue
	 * to see which is of higher priority.
	 */
	template<typename T>
	class UU_API PriorityQueueComparitor
	{
	public:
		static bool FirstOfHigherPriorityThanSecond(const T& keyA, const T& keyB)
		{
			return keyA > keyB;
		}
	};

	/**
	 * These are binary trees where the root always has the key of highest priority.
	 * Efficient insertion is provided as well as removal of the root.
	 * See chapter 7 of Introduction to Algorithms by Rivest, et. al.
	 * 
	 * Note that one weakness of this data-structure is that once a key
	 * is inserted, it's effective priority cannot be changed without possibly
	 * invalidating the integrity of the queue.  To overcome this weakness
	 * (with a less efficient data-structure), use the @ref DynamicPriorityQueue.
	 */
	template<typename T, typename C = PriorityQueueComparitor<T>>
	class UU_API StaticPriorityQueue
	{
	public:
		StaticPriorityQueue()
		{
		}

		virtual ~StaticPriorityQueue()
		{
		}

		/**
		 * This is provided merely for diagnostic purposes to verify that
		 * the heap data-structure is correct.  That is, that the heap
		 * property is satisfied on all nodes of the tree.
		 */
		bool IsValidHeap(unsigned int i = 0) const
		{
			if (this->array.GetSize() == 0)
				return true;

			if (i >= this->array.GetSize())
				return false;

			const T& parentKey = this->array[i];

			if (LeftChild(i) < this->array.GetSize())
			{
				const T& leftKey = this->array[LeftChild(i)];
				if (C::FirstOfHigherPriorityThanSecond(leftKey, parentKey))
					return false;

				if (!this->IsValidHeap(LeftChild(i)))
					return false;
			}

			if (RightChild(i) < this->array.GetSize())
			{
				const T& rightKey = this->array[RightChild(i)];
				if (C::FirstOfHigherPriorityThanSecond(rightKey, parentKey))
					return false;

				if (!this->IsValidHeap(RightChild(i)))
					return false;
			}

			return true;
		}

		/**
		 * Add a key to the binary tree while maintaining the heap priorty of the tree.
		 * Note that the tree always remains balanced by virtual of our representation of
		 * the tree in a contiguous array of nodes.
		 */
		void InsertKey(T key)
		{
			unsigned int i = this->array.GetSize();
			this->array.Push(key);

			while (i != 0)
			{
				unsigned int j = Parent(i);
				if (this->array[j] >= key)
					break;

				this->array[i] = this->array[j];
				this->array[j] = key;
				i = j;
			}
		}

		/**
		 * Return the key in the tree with highest priority.  This is always the root node's key.
		 */
		bool GetHighestPriorityKey(T& key) const
		{
			if (this->array.GetSize() == 0)
				return false;

			key = this->array[0];
			return true;
		}

		/**
		 * Remove the largest key from the tree while maintaining the heap property
		 * of the tree.  This can fail if the heap is empty.
		 */
		bool RemoveHighestPriorityKey(T& key)
		{
			if (this->array.GetSize() == 0)
				return false;

			key = this->array[0];
			this->array[0] = this->array[this->array.GetSize() - 1];
			this->array.Pop();
			if (this->array.GetSize() > 0)
				this->Heapify(0);
			return true;
		}

		/**
		 * Return the number of nodes/keys in this heap.
		 */
		unsigned int GetSize() const
		{
			return this->array.GetSize();
		}

	private:

		/**
		 * Assuming that the sub-trees rooted at the left and right of the
		 * given node are valid heaps, make sure that the sub-tree rooted at
		 * the given node is a valid heap as well.
		 */
		void Heapify(unsigned int i)
		{
			unsigned int j = i;
			const T& parentKey = this->array[i];
			const T* largerLeft = nullptr;
			const T* largerRight = nullptr;

			if (LeftChild(i) < this->array.GetSize())
			{
				const T& leftKey = this->array[LeftChild(i)];
				if (C::FirstOfHigherPriorityThanSecond(leftKey, parentKey))
					largerLeft = &leftKey;
			}
			
			if (RightChild(i) < this->array.GetSize())
			{
				const T& rightKey = this->array[RightChild(i)];
				if (C::FirstOfHigherPriorityThanSecond(rightKey, parentKey))
					largerRight = &rightKey;
			}

			if (largerLeft && largerRight)
			{
				if (C::FirstOfHigherPriorityThanSecond(*largerLeft, *largerRight))
					j = LeftChild(i);
				else
					j = RightChild(i);
			}
			else if (largerLeft)
				j = LeftChild(i);
			else if (largerRight)
				j = RightChild(i);

			if (j != i)
			{
				T key = this->array[i];
				this->array[i] = this->array[j];
				this->array[j] = key;
				this->Heapify(j);
			}
		}

		static unsigned int LeftChild(unsigned int i)
		{
			return 2 * i + 1;
		}

		static unsigned int RightChild(unsigned int i)
		{
			return 2 * (i + 1);
		}

		static unsigned int Parent(unsigned int i)
		{
			return (i - 1) >> 1;
		}

	private:
		DArray<T> array;
	};

	// TODO: Make DynamicPriorityQueue class?
}