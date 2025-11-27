#include "UltraUtilities/Containers/LoopedList.h"

using namespace UU;

//--------------------------------- LoopedList ---------------------------------

LoopedList::LoopedList()
{
	this->mainNode = nullptr;
	this->numNodes = 0;
}

/*virtual*/ LoopedList::~LoopedList()
{
	this->Clear();
}

void LoopedList::InsertNodeAfter(Node* newNode, Node* afterNode /*= nullptr*/)
{
	if (!afterNode)
		afterNode = this->mainNode;
	
	newNode->Couple(afterNode, afterNode->nextNode);
	this->numNodes++;
}

void LoopedList::InsertNodeBefore(Node* newNode, Node* beforeNode /*= nullptr*/)
{
	if (!beforeNode)
		beforeNode = this->mainNode;

	newNode->Couple(beforeNode->prevNode, beforeNode);
	this->numNodes++;
}

void LoopedList::RemoveNode(Node* oldNode)
{
	if (this->mainNode == oldNode)
		this->mainNode = oldNode->nextNode;

	oldNode->Decouple();
	this->numNodes--;
}

LoopedList::Node* LoopedList::GetMainNode()
{
	return this->mainNode;
}

void LoopedList::SetMainNode(Node* node)
{
	this->mainNode = node;
}

void LoopedList::Clear()
{
	while (this->numNodes > 0)
		this->RemoveNode(this->mainNode);
}

unsigned int LoopedList::GetNumNodes() const
{
	return this->numNodes;
}

bool LoopedList::Combine(LoopedList* listA, LoopedList* listB)
{
	if (listA == listB)
		return false;

	if (this == listA || this == listB)
		return false;

	this->Clear();

	if (listA->numNodes == 0 && listB->numNodes == 0)
		return true;

	if (listA->numNodes == 0)
	{
		this->mainNode = listB->mainNode;
		this->numNodes = listB->numNodes;
		listB->mainNode = nullptr;
		listB->numNodes = 0;
		return true;
	}

	if (listB->numNodes == 0)
	{
		this->mainNode = listA->mainNode;
		this->numNodes = listA->numNodes;
		listA->mainNode = nullptr;
		listA->numNodes = 0;
		return true;
	}

	Node* nodeA = listA->mainNode->nextNode;
	Node* nodeB = listB->mainNode->prevNode;

	listA->mainNode->nextNode = listB->mainNode;
	listB->mainNode->prevNode = listA->mainNode;

	nodeA->prevNode = nodeB;
	nodeB->nextNode = nodeA;

	this->mainNode = listA->mainNode;
	this->numNodes = listA->numNodes + listB->numNodes;

	listA->mainNode = nullptr;
	listA->numNodes = 0;

	listB->mainNode = nullptr;
	listB->numNodes = 0;

	return true;
}

//--------------------------------- LoopedList::Node ---------------------------------

LoopedList::Node::Node()
{
	this->nextNode = nullptr;
	this->prevNode = nullptr;
}

/*virtual*/ LoopedList::Node::~Node()
{
}

void LoopedList::Node::Decouple()
{
	if (this->nextNode != this)
		this->nextNode = this->nextNode->nextNode;

	if (this->prevNode != this)
		this->prevNode = this->prevNode->prevNode;

	this->nextNode = nullptr;
	this->prevNode = nullptr;
}

void LoopedList::Node::Couple(Node* beforeNode, Node* afterNode)
{
	UU_ASSERT(beforeNode->nextNode == afterNode);
	UU_ASSERT(afterNode->prevNode == beforeNode);

	beforeNode->nextNode = this;
	afterNode->prevNode = this;

	this->nextNode = afterNode;
	this->prevNode = beforeNode;
}

LoopedList::Node* LoopedList::Node::GetNextNode()
{
	return this->nextNode;
}

LoopedList::Node* LoopedList::Node::GetPrevNode()
{
	return this->prevNode;
}