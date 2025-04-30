#include "UltraUtilities/Containers/HashMap.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Hash Maps", "[HashMap]")
{
	HashMap<int, int> map;

	REQUIRE(map.GetNumPairs() == 0);

	SECTION("Test basic insertion and clearing.")
	{
		for (int i = 0; i < 10; i++)
		{
			map.Insert(i, 2 * i);
			REQUIRE(map.GetNumPairs() == i + 1);
		}

		map.Clear();
		REQUIRE(map.GetNumPairs() == 0);
	}

	SECTION("Test basic search and removal.")
	{
		for (int i = 0; i < 10; i++)
			map.Insert(i, 2 * i);

		for (int i = 0; i < 10; i++)
		{
			int j = -1;
			REQUIRE(map.Find(i, &j));
			REQUIRE(j == 2 * i);
		}
	}

	SECTION("Ranged for-loop.")
	{
		for (int i = 0; i < 10; i++)
			map.Insert(i, 2 * i);

		int i = 0;
		for (auto pair : map)
		{
			REQUIRE(pair.key == i);
			REQUIRE(pair.value == i * 2);
			i++;
		}
	}
}