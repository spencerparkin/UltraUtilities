#include "UltraUtilities/Containers/FibonacciHeap.h"
#include "UltraUtilities/Containers/DArray.hpp"
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
			bool inserted = heap.Insert(j);
			REQUIRE(inserted);
			bool validHeap = heap.IsValid();
			REQUIRE(validHeap);
		}
	}

	SECTION("Merging heaps and removal of minimum.")
	{
		DArray<int> evenNumbersArray;
		DArray<int> oddNumbersArray;

		for (int i = 0; i < 1000; i++)
		{
			if (i % 2 == 0)
				evenNumbersArray.Push(i);
			else
				oddNumbersArray.Push(i);
		}

		random.Shuffle(evenNumbersArray.GetBuffer(), evenNumbersArray.GetSize());
		random.Shuffle(oddNumbersArray.GetBuffer(), oddNumbersArray.GetSize());

		FibonacciHeap evenHeap, oddHeap;

		for (int i : evenNumbersArray)
			evenHeap.Insert(i);

		for (int i : oddNumbersArray)
			oddHeap.Insert(i);

		FibonacciHeap heap;
		heap.Merge(&evenHeap, &oddHeap);

		for (int i = 0; i < 1000; i++)
		{
			int j = -1;
			bool removed = heap.RemoveMinimum(j);
			REQUIRE(removed);
			REQUIRE(j == i);
		}
	}
}