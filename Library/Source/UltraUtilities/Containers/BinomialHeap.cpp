#include "UltraUtilities/Containers/BinomialHeap.h"
#include "UltraUtilities/Containers/HashSet.hpp"
#include "UltraUtilities/Containers/DArray.hpp"

using namespace UU;

//---------------------------------- BinomialHeap ----------------------------------

BinomialHeap::BinomialHeap()
{
	this->rootNode = nullptr;
}

/*virtual*/ BinomialHeap::~BinomialHeap()
{
	this->Clear();
}

bool BinomialHeap::IsValid() const
{
	for (const Node* node = this->rootNode; node; node = node->siblingNode)
		if (node->parentNode)
			return false;

	if (!this->ForAllNodes([](const Node* node) -> bool {
			return node->IsDegreeValid();
		}))
	{
		return false;
	}

	for (const Node* node = this->rootNode; node; node = node->siblingNode)
		if (!node->IsHeapOrdered())
			return false;

	HashSet<unsigned int> degreeSet;
	for (const Node* node = this->rootNode; node; node = node->siblingNode)
	{
		if (degreeSet.Find(node->degree))
			return false;

		degreeSet.Insert(node->degree);
	}

	unsigned int lastTreeHeight = 0;
	unsigned int treeHeight = 0;
	for (const Node* node = this->rootNode; node; node = node->siblingNode)
	{
		if (!node->IsBinomialTree(treeHeight))
			return false;

		if (treeHeight <= lastTreeHeight)
			return false;

		lastTreeHeight = treeHeight;
	}

	return true;
}

void BinomialHeap::Clear()
{
	delete this->rootNode;
	this->rootNode = nullptr;
}

void BinomialHeap::InsertNode(Node* node)
{
	BinomialHeap heapA;
	heapA.rootNode = this->rootNode;
	this->rootNode = nullptr;

	BinomialHeap heapB;
	heapB.rootNode = node;

	this->Merge(&heapA, &heapB);
}

BinomialHeap::Node* BinomialHeap::RemoveMinimalNode()
{
	if (!this->rootNode)
		return nullptr;

	Node* nodeToRemove = nullptr;
	for (Node* node = this->rootNode; node; node = node->siblingNode)
		if (!nodeToRemove || node->IsLessThan(nodeToRemove))
			nodeToRemove = node;

	if (this->rootNode == nodeToRemove)
		this->rootNode = nodeToRemove->siblingNode;
	else
	{
		Node* node = this->rootNode;
		while (node->siblingNode != nodeToRemove)
			node = node->siblingNode;

		node->siblingNode = nodeToRemove->siblingNode;
	}

	nodeToRemove->siblingNode = nullptr;

	BinomialHeap heapA;
	heapA.rootNode = this->rootNode;
	this->rootNode = nullptr;

	BinomialHeap heapB;

	DArray<Node*> nodeArray;
	nodeArray.SetCapacity(nodeToRemove->degree);
	for (Node* childNode = nodeToRemove->childNode; childNode; childNode = childNode->siblingNode)
		nodeArray.Push(childNode);

	nodeToRemove->childNode = nullptr;

	Node* tailNode = nullptr;
	while (nodeArray.GetSize() > 0)
	{
		Node* node = nodeArray.Pop((Node*)nullptr);
		node->siblingNode = nullptr;
		node->parentNode = nullptr;
		if (!tailNode)
			heapB.rootNode = node;
		else
			tailNode->siblingNode = node;
		tailNode = node;
	}

	this->Merge(&heapA, &heapB);
	
	return nodeToRemove;
}

BinomialHeap::Node* BinomialHeap::RemoveRoot()
{
	if (!this->rootNode)
		return nullptr;

	Node* node = this->rootNode;
	this->rootNode = node->siblingNode;
	node->siblingNode = nullptr;
	return node;
}

bool BinomialHeap::AppendNode(Node* node, Node*& lastNode)
{
	if (node->siblingNode)
		return false;

	if (!this->rootNode)
		this->rootNode = node;
	else
	{
		if (!lastNode)
			return false;

		lastNode->siblingNode = node;
	}

	lastNode = node;
	return true;
}

void BinomialHeap::Merge(BinomialHeap* heapA, BinomialHeap* heapB)
{
	this->Clear();

	Node* lastNode = nullptr;

	while (true)
	{
		Node* nodeA = heapA->rootNode;
		Node* nodeB = heapB->rootNode;

		if (!nodeA || !nodeB)
			break;

		if (nodeA->degree <= nodeB->degree)
			this->AppendNode(heapA->RemoveRoot(), lastNode);
		else
			this->AppendNode(heapB->RemoveRoot(), lastNode);
	}

	while (heapA->rootNode)
		this->AppendNode(heapA->RemoveRoot(), lastNode);

	while (heapB->rootNode)
		this->AppendNode(heapB->RemoveRoot(), lastNode);

	Node* currentNode = this->rootNode;
	Node* previousNode = nullptr;

	enum Action
	{
		Unknown,
		Advance,
		Combine
	};

	while (currentNode && currentNode->siblingNode)
	{
		Action action = Action::Unknown;

		if (currentNode->degree != currentNode->siblingNode->degree)
			action = Action::Advance;
		else if (!currentNode->siblingNode->siblingNode)
			action = Action::Combine;
		else if (currentNode->degree == currentNode->siblingNode->siblingNode->degree)
			action = Action::Advance;
		else
			action = Action::Combine;
		
		switch (action)
		{
			case Action::Advance:
			{
				previousNode = currentNode;
				currentNode = currentNode->siblingNode;
				break;
			}
			case Action::Combine:
			{
				Node* childNode = nullptr;
				Node* parentNode = nullptr;

				if (currentNode->IsLessThan(currentNode->siblingNode))
				{
					childNode = currentNode->siblingNode;
					parentNode = currentNode;
					currentNode->siblingNode = currentNode->siblingNode->siblingNode;
				}
				else
				{
					childNode = currentNode;
					parentNode = currentNode->siblingNode;
					if (previousNode)
						previousNode->siblingNode = currentNode->siblingNode;
					else
						this->rootNode = currentNode->siblingNode;
				}

				childNode->parentNode = parentNode;
				childNode->siblingNode = parentNode->childNode;
				parentNode->childNode = childNode;
				parentNode->degree++;
				currentNode = parentNode;

				break;
			}
		}
	}
}

//---------------------------------- BinomialHeap::Node ----------------------------------

BinomialHeap::Node::Node()
{
	this->parentNode = nullptr;
	this->childNode = nullptr;
	this->siblingNode = nullptr;
	this->degree = 0;
}

/*virtual*/ BinomialHeap::Node::~Node()
{
	delete this->childNode;
	delete this->siblingNode;
}

bool BinomialHeap::Node::IsDegreeValid() const
{
	unsigned int numChildren = 0;
	const Node* node = this->childNode;
	while (node)
	{
		numChildren++;
		node = node->siblingNode;
	}

	return numChildren == this->degree;
}

bool BinomialHeap::Node::IsHeapOrdered() const
{
	if (!this->childNode)
		return true;

	for (const Node* node = this->childNode; node; node = node->siblingNode)
	{
		if (node->IsLessThan(this))
			return false;

		if (!node->IsHeapOrdered())
			return false;
	}

	return true;
}

bool BinomialHeap::Node::IsBinomialTree(unsigned int& treeHeight) const
{
	if (this->degree == 0)
	{
		treeHeight = 1;
		return true;
	}

	unsigned int expectedSubTreeHeight = this->degree;
	for (const Node* node = this->childNode; node; node = node->siblingNode)
	{
		unsigned int subTreeHeight = 0;
		if (!node->IsBinomialTree(subTreeHeight))
			return false;

		if (subTreeHeight != expectedSubTreeHeight)
			return false;

		expectedSubTreeHeight--;
	}

	if (expectedSubTreeHeight != 0)
		return false;

	treeHeight = 1 + this->degree;
	return true;
}