#pragma once

#include "UltraUtilities/Defines.h"

namespace UU
{
	/**
	 * This is like a doubly-linked list, but there's no beginning
	 * and no end.  It's a circular list.
	 */
	class UU_API LoopedList
	{
	public:
		LoopedList();
		virtual ~LoopedList();

		class Node;

		/**
		 * Insert the given node (allocated by the caller on the heap) after the other given node.
		 * This class takes ownership of the memory.
		 * 
		 * @param[in,out] newNode This is the node to insert.  It must not already be a member of some other list.
		 * @param[in,out] afterNode The new node is inserted after this.  If not given, insertion is performed after the main node.  It must be a member of this list.
		 */
		void InsertNodeAfter(Node* newNode, Node* afterNode = nullptr);

		/**
		 * Insert the given node (allocated by the caller on the heap) before the other given node.
		 * This class takes ownership of the memory.
		 * 
		 * @param[in,out] newNode This is the node to insert.  It must not already be a member of some other list.
		 * @param[in,out] beforeNode The new node is inserted before this.  If not given, insertion is performed before the main node.  It must be a member of this list.
		 */
		void InsertNodeBefore(Node* newNode, Node* beforeNode = nullptr);

		/**
		 * Remove the given node (assumed to be in this list) from this list.
		 * The caller takes ownership of the memory.
		 */
		void RemoveNode(Node* oldNode);

		/**
		 * Return the node being used to maintain a reference to the entire list.
		 */
		Node* GetMainNode();

		/**
		 * Assign an alternative node as the one maintaining a reference to the entire list.
		 * The caller must be careful to ensure that the given node is actually a member
		 * of this list.  No check is made to ensure that this is the case.
		 */
		void SetMainNode(Node* node);

		/**
		 * Delete all nodes from this list, making it empty.
		 */
		void Clear();

		/**
		 * Return the number of nodes in this list.
		 */
		unsigned int GetNumNodes() const;

		/**
		 * Combine the two given looped-lists into a single looped list,
		 * destroying them in the process.
		 * 
		 * @param[in,out] listA Obsorb this list's nodes into this one, making it empty.
		 * @param[in,out] listB Obsorb this lists's nodes too into this one, making it empty.
		 * @return Failure can occur if both given lists are the same.
		 */
		bool Combine(LoopedList* listA, LoopedList* listB);

		/**
		 * Users of the list are expected to derive from this class.
		 */
		class Node
		{
			friend class LoopedList;

		public:
			Node();
			virtual ~Node();

			void Decouple();
			void Couple(Node* beforeNode, Node* afterNode);

			Node* GetNextNode();
			Node* GetPrevNode();

		public:
			Node* nextNode;
			Node* prevNode;
		};

	protected:
		Node* mainNode;
		unsigned int numNodes;
	};
}