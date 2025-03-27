#include "UltraUtilities/Containers/RBMap.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Red/Black Maps", "[RBMap]")
{
	RBMap<int, int> map;

	REQUIRE(map.GetNumPairs() == 0);

	SECTION("Test basic insertion and clearing.")
	{
		map.Insert(1, 1);
		REQUIRE(map.GetNumPairs() == 1);

		map.Insert(1, 1);
		REQUIRE(map.GetNumPairs() == 1);

		map.Insert(2, 2);
		map.Insert(3, 3);
		REQUIRE(map.GetNumPairs() == 3);
		REQUIRE(map.GetTree().IsBinaryTree());

		map.Clear();
		REQUIRE(map.GetNumPairs() == 0);
	}

	SECTION("Test basic search and removal.")
	{
		map.Insert(1, 1);
		map.Insert(2, 2);
		map.Insert(3, 3);
		map.Insert(4, 4);
		REQUIRE(map.GetNumPairs() == 4);
		REQUIRE(map.Find(3));
		REQUIRE(map.Find(4));
		REQUIRE(map.GetTree().IsBinaryTree());

		map.Remove(3);
		map.Remove(1);
		REQUIRE(map.GetNumPairs() == 2);
		REQUIRE(!map.Find(3));
		REQUIRE(map.Find(4));
		REQUIRE(map.GetTree().IsBinaryTree());
	}

	SECTION("Basic traversal.")
	{
		map.Insert(4, 4);
		map.Insert(3, 3);
		map.Insert(2, 2);
		map.Insert(1, 1);

		RBTreeNode* minNode = map.GetTree().FindMinimum();
		REQUIRE(minNode != nullptr);
		if (minNode)
		{
			REQUIRE(static_cast<RBMapNode<int>*>(minNode)->value == 1);
		}

		RBTreeNode* maxNode = map.GetTree().FindMaximum();
		REQUIRE(maxNode != nullptr);
		if (maxNode)
		{
			REQUIRE(static_cast<RBMapNode<int>*>(maxNode)->value == 4);
		}

		int i = 1;
		RBTreeNode* node = minNode;
		while (node)
		{
			REQUIRE(static_cast<RBMapNode<int>*>(node)->value == i);
			node = node->FindSuccessor();
			i++;
		}
		
		i = 4;
		node = maxNode;
		while (node)
		{
			REQUIRE(static_cast<RBMapNode<int>*>(node)->value == i);
			node = node->FindPredecessor();
			i--;
		}
	}

	SECTION("Ranged for-loop.")
	{
		map.Insert(4, 4);
		map.Insert(3, 3);
		map.Insert(2, 2);
		map.Insert(1, 1);

		int i = 1;
		for (auto pair : map)
		{
			REQUIRE(pair.key == i);
			REQUIRE(pair.value == i);
			i++;
		}

		map.SetIterationDirection(RBMapIterator<int, int>::BACKWARD);
		i = 4;
		for (auto pair : map)
		{
			REQUIRE(pair.key == i);
			REQUIRE(pair.value == i);
			i--;
		}
	}

	SECTION("Remains balanced with many insertions.")
	{
		for (int i = 0; i < 1000; i++)
		{
			map.Insert(i, i);
			REQUIRE(map.GetTree().IsRedBlackTree());
		}
	}
}