#include "UltraUtilities/Containers/FibonacciHeap.h"

using namespace UU;

//---------------------------------- FibonacciHeap ----------------------------------

FibonacciHeap::FibonacciHeap()
{
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
		this->rootList.InsertNodeAfter(childNode);
	}

	// STPTODO: This is where we consolidate the heap and find the new minimal node.  Write it.

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
}

void FibonacciHeap::Clear()
{
	this->rootList.Clear();
}

//---------------------------------- FibonacciHeap::Node ----------------------------------

FibonacciHeap::Node::Node()
{
	this->parentNode = nullptr;
}

/*virtual*/ FibonacciHeap::Node::~Node()
{
}