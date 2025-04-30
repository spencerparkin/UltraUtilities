#include "UltraUtilities/Containers/HashSet.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Hash Sets", "[HashSet]")
{
	HashSet<int> set;

	REQUIRE(set.GetNumKeys() == 0);

	SECTION("Test basic insertion and clearing.")
	{
		for (int i = 0; i < 10; i++)
		{
			set.Insert(i);
			REQUIRE(set.GetNumKeys() == i + 1);
		}

		set.Clear();
		REQUIRE(set.GetNumKeys() == 0);
	}

	SECTION("Test basic search and removal.")
	{
		for (int i = 0; i < 10; i++)
			set.Insert(i);

		for (int i = 0; i < 10; i++)
			REQUIRE(set.Find(i));

		for (int i = 0; i < 10; i++)
		{
			int j = (i + 5) % 10;
			REQUIRE(set.Find(j));
			set.Remove(j);
			REQUIRE(!set.Find(j));
		}
	}

	SECTION("Ranged for-loop.")
	{
		for (int i = 0; i < 10; i++)
			set.Insert(i);

		HashSet<int> otherSet;

		for (int key : set)
		{
			REQUIRE(!otherSet.Find(key));
			REQUIRE(otherSet.Insert(key));
		}

		REQUIRE(otherSet.GetNumKeys() == set.GetNumKeys());
	}
}