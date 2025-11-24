#include "UltraUtilities/Containers/BinomialHeap.h"
#include "UltraUtilities/Containers/HashSet.hpp"

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

		if (treeHeight >= lastTreeHeight)
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

bool BinomialHeap::Merge(BinomialHeap* heapA, BinomialHeap* heapB)
{
	this->Clear();

	Node* currentNode = nullptr;

	while (true)
	{
		Node* nodeA = heapA->rootNode;
		Node* nodeB = heapB->rootNode;

		if (!nodeA || !nodeB)
			break;

		if (nodeA->degree <= nodeB->degree)
		{
			heapA->rootNode = nodeA->siblingNode;
			nodeA->siblingNode = nullptr;
			if (currentNode)
				currentNode->siblingNode = nodeA;
			else
				this->rootNode = nodeA;
			currentNode = nodeA;
		}
		else
		{
			heapB->rootNode = nodeB->siblingNode;
			nodeB->siblingNode = nullptr;
			if (currentNode)
				currentNode->siblingNode = nodeB;
			else
				this->rootNode = nodeB;
			currentNode = nodeB;
		}
	}

	currentNode = this->rootNode;
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
				}

				childNode->parentNode = parentNode;
				childNode->siblingNode = parentNode->childNode;
				parentNode->childNode = childNode;

				break;
			}
		}
	}

	return true;
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
		if (this->IsLessThan(node))
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