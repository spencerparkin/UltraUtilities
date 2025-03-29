#include "UltraUtilities/Containers/List.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Lists", "[list]")
{
	List<int> list;

	REQUIRE(list.GetNumValues() == 0);

	SECTION("Test pushing and popping.")
	{
		list.PushBack(1);
		list.PushBack(2);
		list.PushBack(3);
		REQUIRE(list.GetNumValues() == 3);

		list.PopFront();
		list.PopFront();
		REQUIRE(list.GetNumValues() == 1);
		REQUIRE(list.GetAt(0, 0) == 3);
	}

	SECTION("Test insert before.")
	{
		list.PushBack(1);
		list.PushBack(2);
		list.PushBack(4);
		list.InsertBeforeAt(2, 3);
		REQUIRE(list.GetAt(2, 0) == 3);
	}

	SECTION("Test insert after.")
	{
		list.PushBack(1);
		list.PushBack(3);
		list.PushBack(4);
		list.InsertAfterAt(0, 2);
		REQUIRE(list.GetAt(1, 0) == 2);
	}

	SECTION("Test ranged for-loop.")
	{
		int numValues = 10;

		for (int i = 0; i < numValues; i++)
			list.PushBack(i);

		List<int> otherList;
		for (int i : list)
			otherList.PushFront(i);

		REQUIRE(otherList.GetAt(0, -1) == numValues - 1);
		REQUIRE(otherList.GetAt(numValues - 1, -1) == 0);
	}
}