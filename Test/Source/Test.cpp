#include "Test.h"
#include "UltraUtilities/Containers/RBTree.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Red/Black Trees", "[rbtree]")
{
	RBTree_<int, int> tree;

	REQUIRE(tree.GetNumPairs() == 0);

	SECTION("Test basic insertion and clearing.")
	{
		tree.Insert(1, 1);
		REQUIRE(tree.GetNumPairs() == 1);

		tree.Insert(1, 1);
		REQUIRE(tree.GetNumPairs() == 1);

		tree.Insert(2, 2);
		tree.Insert(3, 3);
		REQUIRE(tree.GetNumPairs() == 3);
		REQUIRE(tree.GetTree().IsBinaryTree());

		tree.Clear();
		REQUIRE(tree.GetNumPairs() == 0);
	}

	SECTION("Test basic search and removal.")
	{
		tree.Insert(1, 1);
		tree.Insert(2, 2);
		tree.Insert(3, 3);
		tree.Insert(4, 4);
		REQUIRE(tree.GetNumPairs() == 4);
		REQUIRE(tree.Find(3));
		REQUIRE(tree.Find(4));
		REQUIRE(tree.GetTree().IsBinaryTree());

		tree.Remove(3);
		tree.Remove(1);
		REQUIRE(tree.GetNumPairs() == 2);
		REQUIRE(!tree.Find(3));
		REQUIRE(tree.Find(4));
		REQUIRE(tree.GetTree().IsBinaryTree());
	}

	// TODO: Write massive test where we add thousands of nodes random,
	//       traverse them all in order, then remove them all one-by-one.
	//       After each addition or removal, verify all tree properties.
}