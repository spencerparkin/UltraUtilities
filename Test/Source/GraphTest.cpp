#include "UltraUtilities/Graph.h"
#include "UltraUtilities/Containers/HashSet.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Graph", "[Graph]")
{
	// Make the graph in figure 25.5 of the book.
	Graph graph;

	class Node : public Graph::Node
	{
	public:
		Node(char name)
		{
			this->name = name;
		}

		virtual ~Node()
		{
		}

		char name;
	};

	auto nodeA = new Node('A');
	auto nodeB = new Node('B');
	auto nodeC = new Node('C');
	auto nodeD = new Node('D');
	auto nodeE = new Node('E');

	graph.AddNode(nodeA);
	graph.AddNode(nodeB);
	graph.AddNode(nodeC);
	graph.AddNode(nodeD);
	graph.AddNode(nodeE);

	class Edge : public Graph::Edge
	{
	public:
		Edge(double distance)
		{
			this->distance = distance;
		}

		virtual ~Edge()
		{
		}

		virtual double GetWeight() const override
		{
			return this->distance;
		}

		double distance;
	};

	auto edgeAB = new Edge(10.0);
	auto edgeAD = new Edge(5.0);
	auto edgeBC = new Edge(1.0);
	auto edgeBD = new Edge(2.0);
	auto edgeCE = new Edge(6.0);
	auto edgeDB = new Edge(3.0);
	auto edgeDC = new Edge(9.0);
	auto edgeDE = new Edge(2.0);
	auto edgeEA = new Edge(7.0);

	edgeAB->Link(nodeA, nodeB);
	edgeAD->Link(nodeA, nodeD);
	edgeBC->Link(nodeB, nodeC);
	edgeBD->Link(nodeB, nodeD);
	edgeCE->Link(nodeC, nodeE);
	edgeDB->Link(nodeD, nodeB);
	edgeDC->Link(nodeD, nodeC);
	edgeDE->Link(nodeD, nodeE);
	edgeEA->Link(nodeE, nodeA);

	graph.AddEdge(edgeAB);
	graph.AddEdge(edgeAD);
	graph.AddEdge(edgeBC);
	graph.AddEdge(edgeBD);
	graph.AddEdge(edgeCE);
	graph.AddEdge(edgeDB);
	graph.AddEdge(edgeDC);
	graph.AddEdge(edgeDE);
	graph.AddEdge(edgeEA);

	SECTION("Test DFS.")
	{
		HashSet<char> nameSet;

		Graph::NodeLambdaVisitor visitor([&nameSet](Graph::Node* node) -> bool
			{
				nameSet.Insert(static_cast<Node*>(node)->name);
				return true;
			});

		graph.DepthFirstSearch(nodeA, &visitor);

		REQUIRE(nameSet.GetNumKeys() == graph.GetNumNodes());
	}

	SECTION("Test BFS.")
	{
		HashSet<char> nameSet;

		Graph::NodeLambdaVisitor visitor([&nameSet](Graph::Node* node) -> bool
			{
				nameSet.Insert(static_cast<Node*>(node)->name);
				return true;
			});

		graph.BreadthFirstSearch(nodeC, &visitor);

		REQUIRE(nameSet.GetNumKeys() == graph.GetNumNodes());
	}

	SECTION("Test Dijkstra.")
	{
		List<Graph::Node*> shortestPath;
		double shortestDistance = 0.0;

		bool success = graph.DijkstrasAlgorithm(nodeA, nodeC, shortestPath, shortestDistance);
		REQUIRE(success);

		REQUIRE(shortestPath.GetNumValues() == 4);
		
		const char* expectedPath = "ADBC";
		for (int i = 0; i < 4; i++)
		{
			Node* node = static_cast<Node*>(shortestPath.GetAt(i, (Graph::Node*)nullptr));
			REQUIRE(node->name == expectedPath[i]);
		}
	}
}