#pragma once

#include "UltraUtilities/Containers/LoopedList.h"
#include "UltraUtilities/Containers/Queue.hpp"

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
		 * Insert the given key into this heap.
		 */
		template<typename T>
		bool Insert(T key)
		{
			return this->InsertNode(new TypedNode<T>(key));
		}

		/**
		 * Return a node in this heap with minimal key.  There can, of course,
		 * be more than one node having the minimal key.  Which of those we return
		 * here is left undefined.
		 */
		Node* FindMinimumNode();

		/**
		 * Find and return, but do not remove, a minimum key in this heap.
		 */
		template<typename T>
		bool FindMinimum(T& key)
		{
			auto node = dynamic_cast<TypedNode<T>*>(this->FindMinimumNode());
			if (!node)
				return false;

			key = node->key;
			return true;
		}

		/**
		 * Remove and return a node in this heap with minimal key.
		 * The caller takes ownership of the memory.  This is the most
		 * expensive operation, because this is the only place where
		 * the heap must become reconsolidated and where we must find
		 * the new minimal node for the next call.
		 */
		Node* RemoveMinimumNode();

		/**
		 * Find, return and remove a minimum key from this heap.
		 */
		template<typename T>
		bool RemoveMinimum(T& key)
		{
			auto node = this->RemoveMinimumNode();
			if (!node)
				return false;

			auto typedNode = dynamic_cast<TypedNode<T>*>(node);
			UU_ASSERT(typedNode);
			key = typedNode->key;
			delete node;
			return true;
		}

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
		 * Return the number of nodes in this heap.
		 */
		unsigned int GetNumNodes() const;

		/**
		 * This is used for debugging purposes to make sure that all
		 * heap properties are being properly maintained.  For now,
		 * I'm not doing a comprehensive check, but this is better
		 * than nothing.
		 */
		bool IsValid() const;

		/**
		 * This can be used to visit all nodes in the heap.
		 */
		template<typename Lambda>
		bool ForAllNodes(Lambda lambda) const
		{
			Queue<const LoopedList*> listQueue;
			listQueue.PushBack(&this->rootList);
			while (listQueue.GetSize() > 0)
			{
				const LoopedList* nodeList = nullptr;
				listQueue.PopFront(nodeList);

				const Node* node = (const Node*)nodeList->GetMainNode();
				for (unsigned int i = 0; i < nodeList->GetNumNodes(); i++)
				{
					if (!lambda(node))
						return false;

					listQueue.PushBack(&node->childList);

					node = (const Node*)node->GetNextNode();
				}
			}

			return true;
		}

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
			virtual bool IsEqualTo(const Node* node) const = 0;

			bool IsHeapOrdered() const;

		protected:
			LoopedList childList;
			Node* parentNode;
		};

		/**
		 * Delegate comparison operations to those of a given type.
		 */
		template<typename T>
		class TypedNode : public Node
		{
		public:
			TypedNode(T key)
			{
				this->key = key;
			}

			virtual ~TypedNode()
			{
			}

			virtual bool IsGreaterThan(const Node* node) const override
			{
				return this->key > static_cast<const TypedNode<T>*>(node)->key;
			}

			virtual bool IsLessThan(const Node* node) const override
			{
				return this->key < static_cast<const TypedNode<T>*>(node)->key;
			}

			virtual bool IsEqualTo(const Node* node) const override
			{
				return this->key == static_cast<const TypedNode<T>*>(node)->key;
			}

		public:
			T key;
		};

	private:
		LoopedList rootList;
		unsigned int numNodes;
	};
}