#include "UltraUtilities/Containers/LinkedList.h"

using namespace UU;

//------------------------------ LinkedList ------------------------------

LinkedList::LinkedList()
{
	this->headNode = nullptr;
	this->tailNode = nullptr;
	this->numNodes = 0;
}

/*virtual*/ LinkedList::~LinkedList()
{
	this->Clear();
}

bool LinkedList::InsertBefore(LinkedListNode* newNode, LinkedListNode* afterNode /*= nullptr*/)
{
	if (!newNode || newNode->list != nullptr)
		return false;

	if (!afterNode)
		afterNode = this->headNode;
	else if (afterNode->list != this)
		return false;
	
	if (afterNode)
		newNode->SpliceIn(afterNode->prevNode, afterNode);
	else
		this->tailNode = newNode;

	if (!newNode->prevNode)
		this->headNode = newNode;

	newNode->list = this;
	this->numNodes++;
	return true;
}

bool LinkedList::InsertAfter(LinkedListNode* newNode, LinkedListNode* beforeNode /*= nullptr*/)
{
	if (!newNode || newNode->list != nullptr)
		return false;

	if (!beforeNode)
		beforeNode = this->tailNode;
	else if (beforeNode->list != this)
		return false;

	if (beforeNode)
		newNode->SpliceIn(beforeNode, beforeNode->nextNode);
	else
		this->headNode = newNode;

	if (!newNode->nextNode)
		this->tailNode = newNode;

	newNode->list = this;
	this->numNodes++;
	return true;
}

bool LinkedList::Remove(LinkedListNode* oldNode)
{
	if (!oldNode || oldNode->list != this)
		return false;

	if (this->headNode == oldNode)
		this->headNode = oldNode->nextNode;

	if (this->tailNode == oldNode)
		this->tailNode = oldNode->prevNode;

	oldNode->SpliceOut();
	oldNode->list = nullptr;
	this->numNodes--;
	return true;
}

void LinkedList::Clear()
{
	while (this->numNodes > 0)
	{
		LinkedListNode* node = this->headNode;
		this->Remove(node);
		delete node;
	}
}

LinkedListNode* LinkedList::GetNodeAt(unsigned int i)
{
	LinkedListNode* node = this->headNode;

	while (node && i > 0)
	{
		node = node->nextNode;
		i--;
	}

	return node;
}

//------------------------------ LinkedListNode ------------------------------

LinkedListNode::LinkedListNode()
{
	this->prevNode = nullptr;
	this->nextNode = nullptr;
	this->list = nullptr;
}

/*virtual*/ LinkedListNode::~LinkedListNode()
{
}

void LinkedListNode::SpliceIn(LinkedListNode* beforeNode, LinkedListNode* afterNode)
{
	if (beforeNode)
		beforeNode->nextNode = this;

	if (afterNode)
		afterNode->prevNode = this;

	this->prevNode = beforeNode;
	this->nextNode = afterNode;
}

void LinkedListNode::SpliceOut()
{
	if (this->nextNode)
		this->nextNode->prevNode = this->prevNode;

	if (this->prevNode)
		this->prevNode->nextNode = this->nextNode;

	this->nextNode = nullptr;
	this->prevNode = nullptr;
}

unsigned int LinkedListNode::GetPosition() const
{
	unsigned int i = 0;
	const LinkedListNode* node = this;
	while (node->prevNode)
	{
		i++;
		node = node->prevNode;
	}

	return i;
}