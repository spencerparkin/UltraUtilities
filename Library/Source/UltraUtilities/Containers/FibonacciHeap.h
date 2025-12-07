#pragma once

#include "UltraUtilities/Containers/LoopedList.h"
#include "UltraUtilities/Containers/List.hpp"

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
		 * If the user decreased the key of a node, then this call must be made
		 * to fixup the heap.  Note that it is not legal to increase the value
		 * of a key in the heap.  If you do, then all bets are off, and the remainder
		 * of behavior of this heap is left undefined.  Note also that no check is
		 * made to make sure that the given node is actually a member of this heap.
		 * It is up to the caller to make sure this is the case.
		 */
		void KeyWasDecreased(Node* node);

		/**
		 * This method can be used to remove any node from the fibonacci heap,
		 * even if it's not one with minimal key.  It is up to the caller to
		 * ensure that this is actually a node within the heap.  If this is not
		 * the case, then the behavior is undefined and heap corruption will ensue.
		 *
		 * @param[in,out] node This is assumed to be a node within this heap.
		 */
		void RemoveNode(Node* node);

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
			List<const LoopedList*> listQueue;
			listQueue.PushBack(&this->rootList);
			while (listQueue.GetNumValues() > 0)
			{
				const LoopedList* nodeList = nullptr;
				listQueue.PopFront(&nodeList);

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
			virtual void MakeUniquelyMinimal() = 0;
			virtual bool IsUniquelyMinimal() const = 0;

			bool IsHeapOrdered() const;

		protected:
			LoopedList childList;
			Node* parentNode;

			// The only purpose of this boolean is to help us obtain the desired time
			// bounds for the method that decreases a key in the heap.  In most cases,
			// I have at least an intuitive understanding of what makes an algorithm
			// efficient, even if I can't follow the proof from start to finish.  In
			// this case, however, I don't get it at all, and am just following the
			// recipe from the book.
			bool mark;
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

			virtual void MakeUniquelyMinimal() override
			{
				this->key = (T)-1;
			}

			virtual bool IsUniquelyMinimal() const override
			{
				return this->key == (T)-1;
			}

		public:
			T key;
		};

	private:
		LoopedList rootList;
		unsigned int numNodes;
	};
}