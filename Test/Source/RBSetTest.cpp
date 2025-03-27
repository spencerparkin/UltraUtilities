#include "UltraUtilities/Containers/RBSet.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Red/Black Sets", "[RBSet]")
{
	RBSet<int> set;

	REQUIRE(set.GetNumKeys() == 0);

	SECTION("Test basic insertion and clearing.")
	{
		set.Insert(1);
		REQUIRE(set.GetNumKeys() == 1);

		set.Insert(1);
		REQUIRE(set.GetNumKeys() == 1);

		set.Insert(2);
		set.Insert(3);
		REQUIRE(set.GetNumKeys() == 3);
		REQUIRE(set.GetTree().IsBinaryTree());

		set.Clear();
		REQUIRE(set.GetNumKeys() == 0);
	}

	SECTION("Test basic search and removal.")
	{
		set.Insert(1);
		set.Insert(2);
		set.Insert(3);
		set.Insert(4);
		REQUIRE(set.GetNumKeys() == 4);
		REQUIRE(set[3]);
		REQUIRE(set[4]);
		REQUIRE(set.GetTree().IsBinaryTree());

		set.Remove(3);
		set.Remove(1);
		REQUIRE(set.GetNumKeys() == 2);
		REQUIRE(!set[3]);
		REQUIRE(set[4]);
		REQUIRE(set.GetTree().IsBinaryTree());
	}

	SECTION("Ranged for-loop.")
	{
		set.Insert(4);
		set.Insert(3);
		set.Insert(2);
		set.Insert(1);

		int i = 1;
		for (int key : set)
		{
			REQUIRE(key == i);
			i++;
		}

		set.SetIterationDirection(RBSetIterator<int>::BACKWARD);
		i = 4;
		for (int key : set)
		{
			REQUIRE(key == i);
			i--;
		}
	}
}