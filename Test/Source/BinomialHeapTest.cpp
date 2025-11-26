#include "UltraUtilities/Containers/BinomialHeap.h"
#include "UltraUtilities/Random.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("BinomialHeaps", "[binomial_heap]")
{
	XorShiftRandom random;
	random.SetSeed(123456789);

	SECTION("Insertion.")
	{
		BinomialHeap heap;

		for (int i = 0; i < 1000; i++)
		{
			int j = random.GetRandomInteger(0, 100);
			heap.Insert(new BinomialHeap::TypedNode<int>(j));
			bool validHeap = heap.IsValid();
			REQUIRE(validHeap);
		}
	}

	SECTION("Removal.")
	{
		BinomialHeap heap;

		for (int i = 0; i < 1000; i++)
		{
			int key = random.GetRandomInteger(0, 100);
			heap.Insert<int>(key);
		}

		int lastKey = -1;
		for (int i = 0; i < 1000; i++)
		{
			int key = -1;
			bool removed = heap.RemoveMinimum<int>(key);
			REQUIRE(removed);
			REQUIRE(key >= lastKey);
			lastKey = key;
			bool validHeap = heap.IsValid();
			REQUIRE(validHeap);
		}
	}
}