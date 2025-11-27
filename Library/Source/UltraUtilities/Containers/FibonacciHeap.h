#pragma once

#include "UltraUtilities/Containers/LoopedList.h"

namespace UU
{
	/**
	 * These are like binomial heaps, but have better amortized performance.
	 */
	class UU_API FibonacciHeap
	{
	public:
		FibonacciHeap();
		virtual ~FibonacciHeap();

		class Node;

		/**
		 * Insert the given node (allocated by the caller) into this heap.
		 * This heap takes ownership of the memory.
		 */
		bool InsertNode(Node* node);

		/**
		 * Return a node in this heap with minimal key.  There can, of course,
		 * be more than one node having the minimal key.  Which of those we return
		 * here is left undefined.
		 */
		Node* FindMinimumNode();

		/**
		 * Remove and return a node in this heap with minimal key.
		 * The caller takes ownership of the memory.  This is the most
		 * expensive operation, because this is the only place where
		 * the heap must become reconsolidated and where we must find
		 * the new minimal node for the next call.
		 */
		Node* RemoveMinimumNode();

		/**
		 * Merge the two given heaps into this heap, destroying them
		 * in the process.
		 */
		void Merge(FibonacciHeap* heapA, FibonacciHeap* heapB);

		/**
		 * Delete all nodes in this heap, making it empty.
		 */
		void Clear();

		/**
		 * Nodes inserted into the heap derive from this class.
		 */
		class Node : public LoopedList::Node
		{
			friend class FibonacciHeap;

		public:
			Node();
			virtual ~Node();

			virtual bool IsGreaterThan(const Node* node) const = 0;
			virtual bool IsLessThan(const Node* node) const = 0;
			virtual bool IsEQualTo(const Node* node) const = 0;

		protected:
			LoopedList childList;
			Node* parentNode;
		};

	private:
		LoopedList rootList;
	};
}