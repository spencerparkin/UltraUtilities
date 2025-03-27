#include "Test.h"
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

	// TODO: Write massive test where we add thousands of nodes random,
	//       traverse them all in order, then remove them all one-by-one.
	//       After each addition or removal, verify all tree properties.
}