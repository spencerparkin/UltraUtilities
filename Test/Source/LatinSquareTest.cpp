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
		}
	}
}