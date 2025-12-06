#include "UltraUtilities/Graph.h"
#include "UltraUtilities/Containers/HashSet.hpp"
#include "UltraUtilities/Containers/BinomialHeap.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Graph", "[Graph]")
{
	// Make the graph in figure 25.5 of the book.
	Graph graph;

	// Unfortunately, this multiple inheritance makes debugging much harder,
	// because a pointer to either base does not reveal any info about this
	// class in the watch variables view.
	class TestNode : public Graph::Node, public BinomialHeap::TypedNode<double>
	{
	public:
		TestNode(char name) : TypedNode(0.0)
		{
			this->name = name;
		}

		virtual ~TestNode()
		{
		}

		virtual double GetWeight() const override
		{
			return this->key;
		}

		virtual void SetWeight(double distance) override
		{
			this->key = distance;
		}

	public:
		char name;
	};

	auto nodeA = new TestNode('A');
	auto nodeB = new TestNode('B');
	auto nodeC = new TestNode('C');
	auto nodeD = new TestNode('D');
	auto nodeE = new TestNode('E');

	graph.AddNode(nodeA);
	graph.AddNode(nodeB);
	graph.AddNode(nodeC);
	graph.AddNode(nodeD);
	graph.AddNode(nodeE);

	class TestEdge : public Graph::Edge
	{
	public:
		TestEdge(double distance)
		{
			this->distance = distance;
		}

		virtual ~TestEdge()
		{
		}

		virtual double GetWeight() const override
		{
			return this->distance;
		}

	public:
		double distance;
	};

	auto edgeAB = new TestEdge(10.0);
	auto edgeAD = new TestEdge(5.0);
	auto edgeBC = new TestEdge(1.0);
	auto edgeBD = new TestEdge(2.0);
	auto edgeCE = new TestEdge(6.0);
	auto edgeDB = new TestEdge(3.0);
	auto edgeDC = new TestEdge(9.0);
	auto edgeDE = new TestEdge(2.0);
	auto edgeEA = new TestEdge(7.0);

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
				nameSet.Insert(static_cast<TestNode*>(node)->name);
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
				nameSet.Insert(static_cast<TestNode*>(node)->name);
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
			TestNode* node = static_cast<TestNode*>(shortestPath.GetAt(i, (Graph::Node*)nullptr));
			REQUIRE(node->name == expectedPath[i]);
		}
	}

	SECTION("Test Dijkstra 2.")
	{
		List<Graph::Node*> shortestPath;
		double shortestDistance = 0.0;

		bool success = graph.DijkstrasAlgorithm2(nodeA, nodeC, shortestPath, shortestDistance);
		REQUIRE(success);

		REQUIRE(shortestPath.GetNumValues() == 4);

		const char* expectedPath = "ADBC";
		for (int i = 0; i < 4; i++)
		{
			TestNode* node = static_cast<TestNode*>(shortestPath.GetAt(i, (Graph::Node*)nullptr));
			REQUIRE(node->name == expectedPath[i]);
		}
	}
}