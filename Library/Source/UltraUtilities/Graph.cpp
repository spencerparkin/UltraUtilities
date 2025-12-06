#include "UltraUtilities/Graph.h"
#include "UltraUtilities/Containers/List.hpp"
#include "UltraUtilities/Containers/PriorityQueue.hpp"
#include "UltraUtilities/Containers/BinomialHeap.h"

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
		node->SetWeight(1.7976931348623158e+308);
		node->considered = false;
	}

	class NodeCompare
	{
	public:
		static bool FirstOfHigherPriorityThanSecond(const Node* keyA, const Node* keyB)
		{
			return keyA->GetWeight() < keyB->GetWeight();
		}
	};

	// We can't use the StaticPriorityQueue here, because the priority
	// of the keys can change while they're present in the queue.
	DynamicPriorityQueue<Node*, NodeCompare> queue;

	nodeA->SetWeight(0.0);
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

			if (adjacentNode->GetWeight() > node->GetWeight() + edgeLength)
			{
				adjacentNode->SetWeight(node->GetWeight() + edgeLength);
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

bool Graph::DijkstrasAlgorithm2(Node* nodeA, Node* nodeB, List<Node*>& shortestPath, double& shortestDistance)
{
	if (!dynamic_cast<BinomialHeap::Node*>(nodeA) || !dynamic_cast<BinomialHeap::Node*>(nodeB))
		return false;

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
		node->SetWeight(1.7976931348623158e+308);
		node->considered = false;
	}

	BinomialHeap queue;
	queue.SetCanExchangeKeys(false);

	nodeA->SetWeight(0.0);
	nodeA->considered = true;
	queue.InsertNode(dynamic_cast<BinomialHeap::Node*>(nodeA));

	while (!queue.IsEmpty())
	{
		Node* node = dynamic_cast<Node*>(queue.RemoveMinimalNode());

		for (Edge* edge : node->adjacencyArray)
		{
			double edgeLength = edge->GetWeight();
			Node* adjacentNode = edge->Follow(node);

			if (!adjacentNode->considered)
			{
				adjacentNode->considered = true;
				queue.InsertNode(dynamic_cast<BinomialHeap::Node*>(adjacentNode));
			}

			if (adjacentNode->GetWeight() > node->GetWeight() + edgeLength)
			{
				adjacentNode->SetWeight(node->GetWeight() + edgeLength);
				adjacentNode->parentNode = node;

				// The correctness of the algorithm ensures, I believe, that
				// the adjacent node is a member of the binomial heap if we
				// get here.  Otherwise, this call would not make sense.
				queue.KeyWasDecreased(dynamic_cast<BinomialHeap::Node*>(adjacentNode));
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
	this->parentNode = nullptr;
}

/*virtual*/ Graph::Node::~Node()
{
}

/*virtual*/ double Graph::Node::GetWeight() const
{
	return 0.0;
}

/*virtual*/ void Graph::Node::SetWeight(double weight)
{
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

/*virtual*/ void Graph::Edge::SetWeight(double weight)
{
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