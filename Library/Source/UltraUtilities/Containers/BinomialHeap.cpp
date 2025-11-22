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
	return false;
}

//---------------------------------- BinomialHeap::Node ----------------------------------

BinomialHeap::Node::Node()
{
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
		if (this->IsLessThan(node) || this->IsEqualTo(node))
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