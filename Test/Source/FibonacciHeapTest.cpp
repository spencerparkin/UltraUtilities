#include "UltraUtilities/Containers/FibonacciHeap.h"
#include "UltraUtilities/Random.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("FibonacciHeaps", "[fibonacci_heap]")
{
	XorShiftRandom random;
	random.SetSeed(123456789);

	SECTION("Insertion.")
	{
		FibonacciHeap heap;

		for (int i = 0; i < 1000; i++)
		{
			int j = random.GetRandomInteger(0, 100);
			heap.Insert(j);
			bool validHeap = heap.IsValid();
			REQUIRE(validHeap);
		}
	}
}