#include "UltraUtilities/Math/LatinSquare.h"
#include "UltraUtilities/Random.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Latin Squares", "[latin_square]")
{
	SECTION("Test randomly generating latin squares.")
	{
		XorShiftRandom random;

		for (int i = 1; i < 10; i++)
		{
			LatinSquare latinSquare(i);
			latinSquare.RandomlyGenerate(random);
			REQUIRE(latinSquare.IsValid());

			for (int row = 0; row < latinSquare.GetSize(); row++)
			{
				for (int col = 0; col < latinSquare.GetSize(); col++)
				{
					int value = -1;
					latinSquare.GetValue(row, col, value);
					printf("%d", value);
					if (col < latinSquare.GetSize() - 1)
						printf(", ");
				}

				printf("\n");
			}

			printf("\n");
		}
	}
}