#include "Test.h"
#include "UltraUtilities/Containers/RBTree.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Red/Black Trees", "[rbtree]")
{
	// Tree should initially be empty.
	RBTree_<int, int> tree;
	REQUIRE(tree.GetNumPairs() == 0);

	// Tree should have pair item after one pair inserted.
	tree.Insert(1, 1);
	REQUIRE(tree.GetNumPairs() == 1);

	// Tree should still have only one pair after trying to insert same key again.
	tree.Insert(1, 1);
	REQUIRE(tree.GetNumPairs() == 1);

	// Tree should have three pairs after adding two more pairs.
	tree.Insert(2, 2);
	tree.Insert(3, 3);
	REQUIRE(tree.GetNumPairs() == 3);

	// Tree should be a valid binary tree at this point.
	REQUIRE(tree.GetTree().IsBinaryTree());

	// Tree should have zero pairs after being cleared.
	tree.Clear();
	REQUIRE(tree.GetNumPairs() == 0);
}