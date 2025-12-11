#include "UltraUtilities/Graph.h"
#include "UltraUtilities/Containers/List.hpp"
#include "UltraUtilities/Containers/PriorityQueue.hpp"
#include "UltraUtilities/Containers/BinomialHeap.h"
#include "UltraUtilities/Containers/FibonacciHeap.h"
#include "UltraUtilities/Containers/HashSet.hpp"
#include "UltraUtilities/Math/Combinatorics.h"

using namespace UU;

//------------------------------------ Graph ------------------------------------

Graph::Graph()
{
}

Graph::Graph(const Graph& graph)
{
	//...
}

Graph::Graph(Graph&& graph) noexcept
{
	for (Node* node : graph.nodeArray)
		this->nodeArray.Push(node);

	for (Edge* edge : graph.edgeArray)
		this->edgeArray.Push(edge);

	graph.nodeArray.SetSize(0);
	graph.edgeArray.SetSize(0);
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

	nodeB->CollectParentChain(shortestPath, true);
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

	nodeB->CollectParentChain(shortestPath, true);
	return true;
}

bool Graph::DijkstrasAlgorithm3(Node* nodeA, Node* nodeB, List<Node*>& shortestPath, double& shortestDistance)
{
	if (!dynamic_cast<FibonacciHeap::Node*>(nodeA) || !dynamic_cast<FibonacciHeap::Node*>(nodeB))
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

	FibonacciHeap queue;

	nodeA->SetWeight(0.0);
	nodeA->considered = true;
	queue.InsertNode(dynamic_cast<FibonacciHeap::Node*>(nodeA));

	while (queue.GetNumNodes() > 0)
	{
		Node* node = dynamic_cast<Node*>(queue.RemoveMinimumNode());

		for (Edge* edge : node->adjacencyArray)
		{
			double edgeLength = edge->GetWeight();
			Node* adjacentNode = edge->Follow(node);

			if (!adjacentNode->considered)
			{
				adjacentNode->considered = true;
				queue.InsertNode(dynamic_cast<FibonacciHeap::Node*>(adjacentNode));
			}

			if (adjacentNode->GetWeight() > node->GetWeight() + edgeLength)
			{
				adjacentNode->SetWeight(node->GetWeight() + edgeLength);
				adjacentNode->parentNode = node;
				queue.KeyWasDecreased(dynamic_cast<FibonacciHeap::Node*>(adjacentNode));
			}
		}
	}

	nodeB->CollectParentChain(shortestPath, true);
	return true;
}

bool Graph::GetConnectedComponents(DArray<Graph*>& connectedComponentsArray)
{
	while (this->nodeArray.GetSize() > 0)
	{
		this->AssignOffsets();

		Graph* graph = new Graph();
		connectedComponentsArray.Push(graph);

		HashSet<unsigned int> edgeOffsetSet;

		NodeLambdaVisitor visitor([graph, &edgeOffsetSet](Node* node) -> bool
			{
				graph->nodeArray.Push(node);
				for (Edge* edge : node->adjacencyArray)
					edgeOffsetSet.Insert(edge->offset);
				return true;
			});

		this->BreadthFirstSearch(this->nodeArray[0], &visitor);

		// This is slow.
		graph->nodeArray.Sort([](Node* nodeA, Node* nodeB) -> int
			{
				return nodeA->offset > nodeB->offset;
			});

		for (Node* node : graph->nodeArray)
			this->nodeArray.QuickRemove(node->offset);

		DArray<unsigned int> edgeOffsetArray;
		for (unsigned int offset : edgeOffsetSet)
			edgeOffsetArray.Push(offset);

		// This is slow.
		edgeOffsetArray.Sort([](int offsetA, int offsetB) -> int
			{
				return offsetA > offsetB;
			});

		for (unsigned int offset : edgeOffsetArray)
		{
			graph->edgeArray.Push(this->edgeArray[offset]);
			this->edgeArray.QuickRemove(offset);
		}
	}

	return true;
}

bool Graph::FindHamiltonianPath(List<Node*>& nodePathList)
{
	nodePathList.Clear();

	if (this->nodeArray.GetSize() == 0)
		return true;

	if (this->nodeArray.GetSize() == 1)
	{
		nodePathList.PushBack(this->nodeArray[0]);
		return true;
	}

	// This is going to be a brute-force attack, and the running time
	// is going to be absolutely abysmal, but it will be technically
	// correct, I believe.

	bool pathFound = false;

	for (unsigned int numberOfChoices = this->edgeArray.GetSize(); numberOfChoices > 0; numberOfChoices--)
	{
		LambdaCombinatorialEnumerator enumerator([this, &nodePathList, &pathFound](const DArray<unsigned int>& offsetArray) -> bool
			{
				DArray<Edge*> chosenEdgeArray;
				chosenEdgeArray.SetCapacity(offsetArray.GetSize());
				for (unsigned int i = 0; i < offsetArray.GetSize(); i++)
					chosenEdgeArray.Push(this->edgeArray[offsetArray[i]]);

				if (this->ConstructPathFromEdges(chosenEdgeArray, nodePathList) && nodePathList.GetNumValues() == this->nodeArray.GetSize())
				{
					pathFound = true;
					return false;
				}

				return true;
			});

		DArray<unsigned int> offsetArray;
		offsetArray.SetCapacity(this->edgeArray.GetSize());
		for (unsigned int i = 0; i < this->edgeArray.GetSize(); i++)
			offsetArray.Push(i);

		enumerator.VisitAllCombinations(numberOfChoices, offsetArray);

		if (pathFound)
			break;
	}

	return pathFound;
}

bool Graph::ConstructPathFromEdges(DArray<Edge*>& givenEdgeArray, List<Node*>& nodePathList)
{
	if (givenEdgeArray.GetSize() == 0)
		return false;

	nodePathList.Clear();
	nodePathList.PushBack(givenEdgeArray[0]->nodeArray[0]);

	while (nodePathList.GetNumValues() < givenEdgeArray.GetSize() + 1)
	{
		unsigned int integrationCount = 0;

		for (unsigned int i = 0; i < givenEdgeArray.GetSize(); i++)
		{
			Edge* edge = givenEdgeArray[i];

			bool integratedEdge = false;

			if (edge->nodeArray[0] == nodePathList.GetFirst())
			{
				nodePathList.PushFront(edge->nodeArray[1]);
				integratedEdge = true;
			}
			else if (edge->nodeArray[1] == nodePathList.GetFirst())
			{
				nodePathList.PushFront(edge->nodeArray[0]);
				integratedEdge = true;
			}
			else if (edge->nodeArray[0] == nodePathList.GetLast())
			{
				nodePathList.PushBack(edge->nodeArray[1]);
				integratedEdge = true;
			}
			else if (edge->nodeArray[1] == nodePathList.GetLast())
			{
				nodePathList.PushBack(edge->nodeArray[0]);
				integratedEdge = true;
			}

			if (integratedEdge)
			{
				givenEdgeArray.QuickRemove(i);
				integrationCount++;
			}
		}

		if (integrationCount == 0)
			return false;
	}

	return true;
}

void Graph::AssignOffsets()
{
	for (unsigned int i = 0; i < this->nodeArray.GetSize(); i++)
		this->nodeArray[i]->offset = i;

	for (unsigned int i = 0; i < this->edgeArray.GetSize(); i++)
		this->edgeArray[i]->offset = i;
}

//------------------------------------ Graph::Node ------------------------------------

Graph::Node::Node()
{
	this->offset = -1;
	this->considered = false;
	this->parentNode = nullptr;
}

/*virtual*/ Graph::Node::~Node()
{
}

void Graph::Node::CollectParentChain(List<Node*>& nodeList, bool reverseOrder)
{
	Node* node = this;

	if (reverseOrder)
	{
		while (node)
		{
			nodeList.PushFront(node);
			node = node->parentNode;
		}
	}
	else
	{
		while (node)
		{
			nodeList.PushBack(node);
			node = node->parentNode;
		}
	}
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
	this->offset = -1;
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