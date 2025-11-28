#include "UltraUtilities/Containers/FibonacciHeap.h"
#include "UltraUtilities/Containers/DArray.hpp"
#include "UltraUtilities/Math/Functions.h"

using namespace UU;

//---------------------------------- FibonacciHeap ----------------------------------

FibonacciHeap::FibonacciHeap()
{
	this->numNodes = 0;
}

/*virtual*/ FibonacciHeap::~FibonacciHeap()
{
	this->Clear();
}

bool FibonacciHeap::InsertNode(Node* node)
{
	if (node->childList.GetNumNodes() > 0)
		return false;

	this->rootList.InsertNodeAfter(node);

	if (node->IsLessThan((Node*)this->rootList.GetMainNode()))
		this->rootList.SetMainNode(node);

	this->numNodes++;
	return true;
}

FibonacciHeap::Node* FibonacciHeap::FindMinimumNode()
{
	return (Node*)this->rootList.GetMainNode();
}

FibonacciHeap::Node* FibonacciHeap::RemoveMinimumNode()
{
	if (this->rootList.GetNumNodes() == 0)
		return nullptr;

	Node* minimalNode = (Node*)this->rootList.GetMainNode();

	this->rootList.RemoveNode(minimalNode);

	while (minimalNode->childList.GetNumNodes() > 0)
	{
		Node* childNode = (Node*)minimalNode->childList.GetMainNode();
		minimalNode->childList.RemoveNode(childNode);
		childNode->parentNode = nullptr;
		this->rootList.InsertNodeAfter(childNode);
	}

	unsigned int nodeArraySize = (unsigned int)NaturalLogarithm(double(this->numNodes));
	nodeArraySize = UU_MAX(1, nodeArraySize);
	DArray<Node*> nodeArray(nodeArraySize);

	for (unsigned int i = 0; i < nodeArray.GetSize(); i++)
		nodeArray[i] = nullptr;

	while (this->rootList.GetNumNodes() > 0)
	{
		Node* rootNodeA = (Node*)this->rootList.GetMainNode();
		this->rootList.RemoveNode(rootNodeA);

		unsigned int degree = rootNodeA->childList.GetNumNodes();
		
		if (nodeArray[degree] == nullptr)
			nodeArray[degree] = rootNodeA;
		else
		{
			Node* rootNodeB = nodeArray[degree];
			nodeArray[degree] = nullptr;

			if (rootNodeA->IsLessThan(rootNodeB))
			{
				rootNodeA->childList.InsertNodeAfter(rootNodeB);
				rootNodeB->parentNode = rootNodeA;
				this->rootList.InsertNodeAfter(rootNodeA);
			}
			else
			{
				rootNodeB->childList.InsertNodeAfter(rootNodeA);
				rootNodeA->parentNode = rootNodeB;
				this->rootList.InsertNodeAfter(rootNodeB);
			}
		}
	}

	Node* nextMinimalNode = nullptr;
	for (Node* node : nodeArray)
	{
		if (node)
		{
			this->rootList.InsertNodeAfter(node);
			if (!nextMinimalNode || node->IsLessThan(nextMinimalNode))
				nextMinimalNode = node;
		}
	}

	if (nextMinimalNode)
		this->rootList.SetMainNode(nextMinimalNode);

	this->numNodes--;
	return minimalNode;
}

void FibonacciHeap::Merge(FibonacciHeap* heapA, FibonacciHeap* heapB)
{
	this->Clear();

	Node* minimalNodeA = (Node*)heapA->rootList.GetMainNode();
	Node* minimalNodeB = (Node*)heapB->rootList.GetMainNode();

	this->rootList.Combine(&heapA->rootList, &heapB->rootList);

	if (minimalNodeA->IsLessThan(minimalNodeB))
		this->rootList.SetMainNode(minimalNodeA);
	else
		this->rootList.SetMainNode(minimalNodeB);

	this->numNodes = heapA->numNodes + heapB->numNodes;
}

void FibonacciHeap::Clear()
{
	this->rootList.Clear();
	this->numNodes = 0;
}

unsigned int FibonacciHeap::GetNumNodes() const
{
	return this->numNodes;
}

bool FibonacciHeap::IsValid() const
{
	unsigned int nodeCount = 0;
	this->ForAllNodes([&nodeCount](const Node* node) -> bool
		{
			nodeCount++;
			return true;
		});

	if (this->numNodes != nodeCount)
		return false;

	const Node* rootNode = (const Node*)this->rootList.GetMainNode();
	for (unsigned int i = 0; i < this->rootList.GetNumNodes(); i++)
	{
		if (!rootNode->IsHeapOrdered())
			return false;

		if (rootNode->parentNode != nullptr)
			return false;

		rootNode = (const Node*)rootNode->GetNextNode();
	}

	return true;
}

//---------------------------------- FibonacciHeap::Node ----------------------------------

FibonacciHeap::Node::Node()
{
	this->parentNode = nullptr;
}

/*virtual*/ FibonacciHeap::Node::~Node()
{
}

bool FibonacciHeap::Node::IsHeapOrdered() const
{
	const Node* childNode = (const Node*)this->childList.GetMainNode();
	for (unsigned int i = 0; i < this->childList.GetNumNodes(); i++)
	{
		if (childNode->IsLessThan(this))
			return false;

		if (!childNode->IsHeapOrdered())
			return false;
	}

	return true;
}