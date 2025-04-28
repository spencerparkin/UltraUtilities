#pragma once

#include "UltraUtilities/Containers/LinkedList.h"

namespace UU
{
	template<typename V> class ListNode;
	template<typename V> class ListIterator;

	/**
	 * This is a templatized wrapper for the @ref LinkedList class.
	 * It is provided as a more convenient way to use the list.
	 * Note that pushing and popping are fast, but operations that
	 * or insert or remove from the middle of the list are slow.
	 * Consider using a dynamic array instead if that's a problem.
	 */
	template<typename V>
	class UU_API List
	{
	public:
		List()
		{
			this->iterationDirection = ListIterator<V>::FORWARD;
		}

		virtual ~List()
		{
		}

		/**
		 * Remove all values from this list.
		 */
		void Clear()
		{
			this->list.Clear();
		}

		/**
		 * Append a value to the list.
		 */
		void PushFront(V value)
		{
			this->list.InsertBefore(new ListNode<V>(value));
		}

		/**
		 * Prepend a value to the list.
		 */
		void PushBack(V value)
		{
			this->list.InsertAfter(new ListNode<V>(value));
		}

		/**
		 * Remove a value from the start of the list.
		 */
		bool PopFront(V* value = nullptr)
		{
			if (this->list.GetNumNodes() == 0)
				return false;
			auto node = static_cast<ListNode<V>*>(this->list.GetHead());
			if (value)
				*value = node->value;
			this->list.Remove(node);
			delete node;
			return true;
		}

		/**
		 * Remove a value from the end of the list.
		 */
		bool PopBack(V* value = nullptr)
		{
			if (this->list.GetNumNodes() == 0)
				return false;
			auto node = static_cast<ListNode<V>*>(this->list.GetTail());
			if (value)
				*value = node->value;
			this->list.Remove(node);
			delete node;
			return true;
		}

		/**
		 * Set the value of list item i.  Note that this is slow.
		 */
		bool SetAt(unsigned int i, V value)
		{
			auto node = static_cast<ListNode<V>*>(this->list.GetNodeAt(i));
			if (!node)
				return false;
			node->value = value;
			return true;
		}

		/**
		 * Get the value of list item i.  Note that this is slow.
		 */
		bool GetAt(unsigned int i, V* value)
		{
			auto node = static_cast<ListNode<V>*>(this->list.GetNodeAt(i));
			if (!node)
				return false;
			*value = node->value;
			return true;
		}

		/**
		 * Get the value of list item i.  Note that this is slow.
		 * 
		 * @param i This is the index (or offset) from the start of the list to where the desired item is located.
		 * @param defaultValue This is the value to return if the given index is out of range.
		 * @return The desired value or the default value is returned.
		 */
		V GetAt(unsigned int i, V defaultValue)
		{
			V value = defaultValue;
			auto node = static_cast<ListNode<V>*>(this->list.GetNodeAt(i));
			if (node)
				value = node->value;
			return value;
		}

		/**
		 * Insert a value before the one at the given position.
		 */
		bool InsertBeforeAt(unsigned int i, V value)
		{
			auto node = static_cast<ListNode<V>*>(this->list.GetNodeAt(i));
			if (!node)
				return false;
			return this->list.InsertBefore(new ListNode<V>(value), node);
		}

		/**
		 * Insert a value after the one at the given position.
		 */
		bool InsertAfterAt(unsigned int i, V value)
		{
			auto node = static_cast<ListNode<V>*>(this->list.GetNodeAt(i));
			if (!node)
				return false;
			return this->list.InsertAfter(new ListNode<V>(value), node);
		}

		/**
		 * Indicate how many values are presently in the list.
		 */
		unsigned int GetNumValues() const { return this->list.GetNumNodes(); }

		/**
		 * This is provided to support the ranged for-loop syntax.
		 */
		ListIterator<V> begin()
		{
			return ListIterator<V>(&this->list, this->iterationDirection);
		}

		/**
		 * This is the end sentinal for the ranged for-loop support.
		 */
		LinkedListNode* end()
		{
			return nullptr;
		}

		/**
		 * Get at the list that is wrapped by this class template.
		 */
		LinkedList* GetList()
		{
			return &this->list;
		}

	private:
		LinkedList list;
		ListIterator<V>::Direction iterationDirection;
	};

	/**
	 * This is used internally by the @ref List class.
	 */
	template<typename V>
	class ListNode : public LinkedListNode
	{
	public:
		ListNode(V value)
		{
			this->value = value;
		}

		virtual ~ListNode()
		{
		}

	public:
		V value;
	};

	/**
	 * This is used internally by the @ref List class to support the ranged for-loop.
	 */
	template<typename V>
	class ListIterator
	{
	public:
		enum Direction
		{
			FORWARD,
			BACKWARD
		};

		ListIterator(LinkedList* list, Direction direction)
		{
			this->direction = direction;
			switch (direction)
			{
			case Direction::FORWARD:
				this->node = list->GetHead();
				break;
			case Direction::BACKWARD:
				this->node = list->GetTail();
				break;
			default:
				this->node = nullptr;
				break;
			}
		}

		void operator++()
		{
			switch (this->direction)
			{
			case FORWARD:
				this->node = this->node->GetNext();
				break;
			case BACKWARD:
				this->node = this->node->GetPrev();
				break;
			}
		}

		bool operator==(LinkedListNode* node)
		{
			return this->node == node;
		}

		V operator*()
		{
			return static_cast<ListNode<V>*>(this->node)->value;
		}

	private:
		LinkedListNode* node;
		Direction direction;
	};
}