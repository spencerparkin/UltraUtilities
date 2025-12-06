#include "UltraUtilities/Containers/BinomialHeap.h"
#include "UltraUtilities/Containers/DArray.hpp"
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

	SECTION("Decrease key.")
	{
		BinomialHeap heap;
		heap.SetCanExchangeKeys(true);

		DArray<BinomialHeap::TypedNode<int>*> nodeArray;
		for (int i = 0; i < 1000; i++)
		{
			int key = random.GetRandomInteger(0, 100);
			auto node = new BinomialHeap::TypedNode<int>(key);
			heap.InsertNode(node);
			nodeArray.Push(node);
		}

		for (auto node : nodeArray)
		{
			if (node->key == 0)
				continue;
			int key = random.GetRandomInteger(0, node->key - 1);
			REQUIRE(key < node->key);
			BinomialHeap::TypedNode<int> nodeWithLesserKey(key);
			bool decreased = heap.DecreaseKey(node, &nodeWithLesserKey);
			REQUIRE(decreased);
			bool validHeap = heap.IsValid();
			REQUIRE(validHeap);
		}
	}

	SECTION("Random removal.")
	{
		BinomialHeap heap;
		heap.SetCanExchangeKeys(true);

		for (int i = 0; i < 1000; i++)
		{
			int key = random.GetRandomInteger(0, 100);
			auto node = new BinomialHeap::TypedNode<int>(key);
			heap.InsertNode(node);
		}

		while (!heap.IsEmpty())
		{
			BinomialHeap::Node* nodeToRemove = nullptr;
			heap.ForAllNodes([&nodeToRemove, &random](BinomialHeap::Node* node) -> bool
				{
					if (random.CoinToss())
					{
						nodeToRemove = node;
						return false;
					}
					return true;
				});
			
			if (nodeToRemove)
			{
				heap.RemoveNode(nodeToRemove);
				bool validHeap = heap.IsValid();
				REQUIRE(validHeap);
			}
		}
	}
}