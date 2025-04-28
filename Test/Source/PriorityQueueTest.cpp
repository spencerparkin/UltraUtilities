#include "UltraUtilities/Containers/PriorityQueue.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Static Priority Queues", "[StaticPriorityQueue]")
{
	StaticPriorityQueue<int> queue;

	SECTION("Load and dump a queue.")
	{
		int count = 100;

		for (int i = 0; i < count; i++)
		{
			queue.InsertKey(i);
			REQUIRE(queue.IsValidHeap());
		}

		for (int i = count - 1; i >= 0; i--)
		{
			int j = 0;
			bool removed = queue.RemoveHighestPriorityKey(j);
			REQUIRE(removed);
			REQUIRE(queue.IsValidHeap());
			REQUIRE(j == i);
		}
	}
}