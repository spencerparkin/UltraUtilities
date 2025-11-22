#pragma once

#include "LinkedList.h"

namespace UU
{
	/**
	 * This is just a simple queue based on a linked list.
	 * It can be a FIFO queue or a FILO queue.
	 */
	template<typename T>
	class Queue
	{
	private:
		class Node : public LinkedListNode
		{
		public:
			Node(T data)
			{
				this->data = data;
			}

			virtual ~Node()
			{
			}

			T data;
		};

	public:
		Queue()
		{
		}

		virtual ~Queue()
		{
			this->Clear();
		}

		unsigned int GetSize() const
		{
			return this->linkedList.GetNumNodes();
		}

		void Clear()
		{
			this->linkedList.Clear();
		}

		void PushBack(T data)
		{
			this->linkedList.InsertAfter(new Node(data));
		}

		void PushFront(T data)
		{
			this->linkedList.InsertBefore(new Node(data));
		}

		bool PopBack(T& data)
		{
			if (this->linkedList.GetNumNodes() == 0)
				return false;

			Node* node = (Node*)this->linkedList.GetTail();
			this->linkedList.Remove(node);
			data = node->data;
			delete node;
			return true;
		}

		bool PopFront(T& data)
		{
			if (this->linkedList.GetNumNodes() == 0)
				return false;

			Node* node = (Node*)this->linkedList.GetHead();
			this->linkedList.Remove(node);
			data = node->data;
			delete node;
			return true;
		}

	private:
		LinkedList linkedList;
	};
}