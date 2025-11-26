#include "UltraUtilities/Containers/BinomialHeap.h"
#include "UltraUtilities/Random.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("BinomialHeaps", "[binomial_heap]")
{
	XorShiftRandom random;
	random.SetSeed(1);

	SECTION("Insertion.")
	{
		BinomialHeap heap;

		for (int i = 0; i < 100; i++)
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

		for (int i = 0; i < 100; i++)
		{
			int j = random.GetRandomInteger(0, 100);
			heap.Insert(new BinomialHeap::TypedNode<int>(j));
		}

		int lastKey = 200;
		for (int i = 0; i < 100; i++)
		{
			auto node = static_cast<BinomialHeap::TypedNode<int>*>(heap.RemoveMinimal());
			REQUIRE(node != nullptr);
			REQUIRE(node->key <= lastKey);
			lastKey = node->key;
		}
	}
}