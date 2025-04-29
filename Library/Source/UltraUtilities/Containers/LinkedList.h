#pragma once

#include "UltraUtilities/Defines.h"

namespace UU
{
	class LinkedListNode;

	/**
	 * These are doubly linked lists.
	 */
	class UU_API LinkedList
	{
	public:
		LinkedList();
		virtual ~LinkedList();

		/**
		 * Insert the given node before the other given node in this list.
		 * This class takes ownership of the memory.
		 * 
		 * @param[in] newNode This must be a node allocated by the caller and not already a member of some other list.
		 * @param[in] afterNode If given, this node will be after the new node; otherwise, the new node becomes the first in the list.
		 */
		bool InsertBefore(LinkedListNode* newNode, LinkedListNode* afterNode = nullptr);

		/**
		 * Insert the given node after the other given node in the list.
		 * This class takes ownership of the memory.
		 * 
		 * @param[in] newNode This must be a node allocated by the caller and not already a member of some other list.
		 * @param[in] beforeNode If given, this node will be before the new node; otherweise, the new node becomes the last in the list.
		 */
		bool InsertAfter(LinkedListNode* newNode, LinkedListNode* beforeNode = nullptr);

		/**
		 * Remove the given node from this list.  The caller takes ownership of the memory.
		 * 
		 * @param[in] oldNode This is the node to remove.  It must be a member of this list.
		 */
		bool Remove(LinkedListNode* oldNode);

		/**
		 * Delete all nodes form this list, making it empty.
		 */
		void Clear();

		/**
		 * Get the node in this list at the given position.  Note that this is
		 * not a very efficient operation.
		 * 
		 * @param[in] i This is a zero-based index (or offset) into the list.
		 * @return The node at offset i is returned.  If i is out of range, then null is returned.
		 */
		LinkedListNode* GetNodeAt(unsigned int i);

		const LinkedListNode* GetNodeAt(unsigned int i) const
		{
			return const_cast<LinkedList*>(this)->GetNodeAt(i);
		}

		LinkedListNode* GetHead() { return this->headNode; }
		LinkedListNode* GetTail() { return this->tailNode; }

		const LinkedListNode* GetHead() const { return this->headNode; }
		const LinkedListNode* GetTail() const { return this->tailNode; }

		unsigned int GetNumNodes() const { return this->numNodes; }

	private:
		LinkedListNode* headNode;
		LinkedListNode* tailNode;
		unsigned int numNodes;
	};

	/**
	 * These are members of a doubly linked list.
	 */
	class UU_API LinkedListNode
	{
		friend class LinkedList;

	public:
		LinkedListNode();
		virtual ~LinkedListNode();

		void SpliceIn(LinkedListNode* beforeNode, LinkedListNode* afterNode);
		void SpliceOut();

		LinkedListNode* GetNext() { return this->nextNode; }
		LinkedListNode* GetPrev() { return this->prevNode; }

		const LinkedListNode* GetNext() const { return this->nextNode; }
		const LinkedListNode* GetPrev() const { return this->prevNode; }

		LinkedList* GetList() { return this->list; }

		const LinkedList* GetList() const { return this->list; }

	private:
		LinkedList* list;
		LinkedListNode* prevNode;
		LinkedListNode* nextNode;
	};
}