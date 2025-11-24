#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/Queue.hpp"

namespace UU
{
	/**
	 * This is a type of heap or priority queue that supports merging.
	 */
	class UU_API BinomialHeap
	{
	public:
		BinomialHeap();
		virtual ~BinomialHeap();

		/**
		 * This is used for debugging purposes to verify that what we
		 * have here is a valid binomial heap (i.e., one that satisfies
		 * all of the binomial heap properties.)
		 */
		bool IsValid() const;

		/**
		 * Delete all the nodes in this heap, making it empty.
		 */
		void Clear();

		/**
		 * This is a fundamental operation of the heap as many heap
		 * operations are based upon it.  Note that both given heaps
		 * will be empty when this function returns, their contents
		 * having been transfered into this one heap.  Also note that
		 * this is a commutative operation.
		 */
		bool Merge(BinomialHeap* heapA, BinomialHeap* heapB);

		/**
		 * This can be used to walk the entire heap.
		 */
		template<typename Lambda>
		bool ForAllNodes(Lambda lambda) const
		{
			if (!this->rootNode)
				return false;

			Queue<Node*> queue;
			queue.PushBack(this->rootNode);
			while (queue.GetSize() > 0)
			{
				Node* node = nullptr;
				queue.PopFront(node);

				if (!lambda(node))
					return false;

				if (node->siblingNode)
					queue.PushBack(node->siblingNode);

				if (node->childNode)
					queue.PushBack(node->childNode);
			}

			return true;
		}

		/**
		 * These are the contents of the binomial heap.  The storage and
		 * nature of the key is up to the derived class.  The main requirement,
		 * of course, is consistency in the comparison operations.  They should
		 * be implemented such that there are no contradiction.  Transitivity,
		 * for example, should hold.
		 */
		class Node
		{
			friend class BinomialHeap;

		public:
			Node();
			virtual ~Node();

			bool IsDegreeValid() const;
			bool IsHeapOrdered() const;
			bool IsBinomialTree(unsigned int& treeHeight) const;

			virtual bool IsLessThan(const Node* node) const = 0;
			virtual bool IsGreaterThan(const Node* node) const = 0;
			virtual bool IsEqualTo(const Node* node) const = 0;

		private:
			Node* parentNode;
			Node* childNode;
			Node* siblingNode;
			unsigned int degree;
		};

		/**
		 * This template class is provided for convenience.  It just delegates
		 * the comparison operations to whatever key type is desired.
		 */
		template<typename T>
		class TypedNode : public Node
		{
		public:
			TypedNode()
			{
			}

			virtual ~TypedNode()
			{
			}

			virtual bool IsLessThan(const Node* node) const override
			{
				return this->key < static_cast<TypedNode<T>*>(node)->key;
			}

			virtual bool IsGreaterThan(const Node* node) const override
			{
				return this->key > static_cast<TypedNode<T>*>(node)->key;
			}

			virtual bool IsEqualTo(const Node* node) const override
			{
				return this->key == static_cast<TypedNode<T>*>(node)->key;
			}

		public:
			T key;
		};

	private:

		Node* rootNode;
	};
}