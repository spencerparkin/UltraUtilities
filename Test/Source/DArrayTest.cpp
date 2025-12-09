#include "UltraUtilities/Containers/DArray.hpp"
#include "UltraUtilities/Containers/PriorityQueue.hpp"
#include "UltraUtilities/Random.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Dynamic Arrays", "[darray]")
{
	DArray<int> array(10);

	REQUIRE(array.GetSize() == 10);
	REQUIRE(array.GetCapacity() == 10);

	SECTION("Test push and pop.")
	{
		array.SetSize(0);
		REQUIRE(array.GetSize() == 0);

		for (int i = 0; i < 10; i++)
			array.Push(i);

		REQUIRE(array.GetSize() == 10);
		REQUIRE(array.GetCapacity() == 10);

		array.Push(-1);

		REQUIRE(array.GetSize() == 11);
		REQUIRE(array.GetCapacity() > 10);

		for (int i = 0; i < 10; i++)
		{
			REQUIRE(array[i] == i);
		}

		REQUIRE(array.Pop(0) == -1);

		for (int i = 0; i < 10; i++)
		{
			REQUIRE(array.Pop(-1) == 9 - i);
		}

		REQUIRE(array.GetSize() == 0);
		REQUIRE(array.GetCapacity() > 10);
	}

	SECTION("Test array copy.")
	{
		array.SetSize(0);
		for (int i = 0; i < 10; i++)
			array.Push(i);

		DArray<int> arrayA(array);
		REQUIRE(arrayA.GetSize() == array.GetSize());

		int arrayB[] = { 1, 2, 3, 4, 5 };
		int arrayBSize = sizeof(arrayB) / sizeof(int);
		DArray<int> arrayC(arrayB, arrayBSize);
		REQUIRE(arrayC.GetSize() == 5);
		REQUIRE(arrayC[4] == 5);

		DArray<int> arrayD = DArray<int>(arrayB, arrayBSize);
		REQUIRE(arrayD.GetSize() == 5);
		REQUIRE(arrayD[4] == 5);
	}

	SECTION("Test bubble sort.")
	{
		array.SetSize(0);
		for (int i = 0; i < 10; i++)
			array.Push(i);

		XorShiftRandom random;
		random.SetSeed(4321);

		random.Shuffle(array.GetBuffer(), array.GetSize());

		array.Sort([](int intA, int intB) -> int {
			return intA < intB ? -1 : 1;
		});

		for (int i = 0; i < 10; i++)
		{
			REQUIRE(i == array[i]);
		}
	}

	SECTION("Test heap sort.")
	{
		array.SetSize(0);
		for (int i = 0; i < 10; i++)
			array.Push(i);

		XorShiftRandom random;
		random.SetSeed(4321);

		random.Shuffle(array.GetBuffer(), array.GetSize());

		class Comparitor
		{
		public:
			static bool FirstOfHigherPriorityThanSecond(const int& keyA, const int& keyB)
			{
				return keyA < keyB;
			}
		};

		StaticPriorityQueue<int, Comparitor> queue;
		while (array.GetSize() > 0)
		{
			int i = -1;
			array.Pop(&i);
			queue.InsertKey(i);
		}

		while (queue.GetSize() > 0)
		{
			int i = -1;
			queue.RemoveHighestPriorityKey(i);
			array.Push(i);
		}

		for (int i = 0; i < 10; i++)
		{
			REQUIRE(i == array[i]);
		}
	}
}