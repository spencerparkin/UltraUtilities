#include "UltraUtilities/Graph.h"
#include "UltraUtilities/Containers/List.hpp"
#include "UltraUtilities/Containers/PriorityQueue.hpp"

using namespace UU;

//------------------------------------ Graph ------------------------------------

Graph::Graph()
{
}

/*virtual*/ Graph::~Graph()
{
	this->Clear();
}

void Graph::AddNode(Node* node)
{
	this->nodeArray.Push(node);
}

void Graph::AddEdge(Edge* edge)
{
	this->edgeArray.Push(edge);
}

void Graph::Clear()
{
	for (Node* node : this->nodeArray)
		delete node;

	for (Edge* edge : this->edgeArray)
		delete edge;

	this->nodeArray.SetSize(0);
	this->edgeArray.SetSize(0);
}

Graph::Node* Graph::GetNode(unsigned int i)
{
	if (i >= this->nodeArray.GetSize())
		return nullptr;

	return this->nodeArray[i];
}

Graph::Edge* Graph::GetEdge(unsigned int i)
{
	if (i >= this->edgeArray.GetSize())
		return nullptr;

	return this->edgeArray[i];
}

unsigned int Graph::GetNumNodes() const
{
	return this->nodeArray.GetSize();
}


unsigned int Graph::GetNumEdges() const
{
	return this->edgeArray.GetSize();
}

bool Graph::DepthFirstSearch(Node* node, NodeVisitor* visitor)
{
	if (!node || !visitor)
		return false;

	this->ClearTraversalFlags();

	node->considered = true;

	return node->DepthFirstSearchRecursive(visitor);
}

bool Graph::BreadthFirstSearch(Node* node, NodeVisitor* visitor)
{
	if (!node || !visitor)
		return false;

	this->ClearTraversalFlags();

	List<Node*> queue;
	queue.PushBack(node);
	node->considered = true;

	while (queue.GetNumValues() > 0)
	{
		queue.PopFront(&node);

		if (!visitor->VisitNode(node))
			return false;

		for (Edge* edge : node->adjacencyArray)
		{
			Node* adjacentNode = edge->Follow(node);
			if (!adjacentNode->considered)
			{
				queue.PushBack(adjacentNode);
				adjacentNode->considered = true;
			}
		}
	}

	return true;
}

void Graph::ClearTraversalFlags()
{
	for (Node* node : this->nodeArray)
		node->considered = false;
}

bool Graph::DijkstrasAlgorithm(Node* nodeA, Node* nodeB, List<Node*>& shortestPath, double& shortestDistance)
{
	shortestPath.Clear();
	shortestDistance = 0.0;

	if (nodeA == nodeB)
	{
		shortestPath.PushBack(nodeA);
		return true;
	}

	for (Node* node : this->nodeArray)
	{
		node->parentNode = nullptr;
		node->distance = 1.7976931348623158e+308;
		node->considered = false;
	}

	class NodeCompare
	{
	public:
		static bool FirstOfHigherPriorityThanSecond(const Node* keyA, const Node* keyB)
		{
			return keyA->distance < keyB->distance;
		}
	};

	// I don't think that we can use the StaticPriorityQueue here, because the
	// priority of the keys can change while they're present in the queue.
	DynamicPriorityQueue<Node*, NodeCompare> queue;

	nodeA->distance = 0.0;
	nodeA->considered = true;
	queue.InsertKey(nodeA);

	while (queue.GetSize() > 0)
	{
		Node* node = nullptr;
		queue.RemoveHighestPriorityKey(node);

		for (Edge* edge : node->adjacencyArray)
		{
			double edgeLength = edge->GetWeight();
			Node* adjacentNode = edge->Follow(node);

			if (adjacentNode->distance > node->distance + edgeLength)
			{
				adjacentNode->distance = node->distance + edgeLength;
				adjacentNode->parentNode = node;
			}

			if (!adjacentNode->considered)
			{
				adjacentNode->considered = true;
				queue.InsertKey(adjacentNode);
			}
		}
	}

	if (nodeB->parentNode == nullptr)
		return false;

	Node* node = nodeB;
	shortestPath.PushFront(node);

	do
	{
		node = node->parentNode;
		shortestPath.PushFront(node);
	} while (node != nodeA);

	return true;
}

//------------------------------------ Graph::Node ------------------------------------

Graph::Node::Node()
{
	this->considered = false;
	this->distance = 0.0;
	this->parentNode = nullptr;
}

/*virtual*/ Graph::Node::~Node()
{
}

/*virtual*/ double Graph::Node::GetWeight() const
{
	return 0.0;
}

bool Graph::Node::DepthFirstSearchRecursive(NodeVisitor* visitor)
{
	if (!visitor->VisitNode(this))
		return false;

	for (Edge* edge : this->adjacencyArray)
	{
		Node* adjacentNode = edge->Follow(this);
		if (!adjacentNode->considered)
		{
			adjacentNode->considered = true;
			if (!adjacentNode->DepthFirstSearchRecursive(visitor))
				return false;
		}
	}

	return true;
}

//------------------------------------ Graph::Edge ------------------------------------

Graph::Edge::Edge()
{
	this->nodeArray[0] = nullptr;
	this->nodeArray[1] = nullptr;
}

/*virtual*/ Graph::Edge::~Edge()
{
}

/*virtual*/ double Graph::Edge::GetWeight() const
{
	return 0.0;
}

bool Graph::Edge::Link(Node* nodeA, Node* nodeB)
{
	if (this->nodeArray[0] || this->nodeArray[1])
		return false;

	this->nodeArray[0] = nodeA;
	this->nodeArray[1] = nodeB;

	nodeA->adjacencyArray.Push(this);
	nodeB->adjacencyArray.Push(this);

	return true;
}

bool Graph::Edge::Unlink()
{
	if (!this->nodeArray[0] || !this->nodeArray[1])
		return false;

	this->nodeArray[0]->adjacencyArray.Remove(this);
	this->nodeArray[1]->adjacencyArray.Remove(this);

	this->nodeArray[0] = nullptr;
	this->nodeArray[1] = nullptr;

	return true;
}

Graph::Node* Graph::Edge::Follow(Node* node)
{
	if (node == this->nodeArray[0])
		return this->nodeArray[1];
	if (node == this->nodeArray[1])
		return this->nodeArray[0];
	return nullptr;
}