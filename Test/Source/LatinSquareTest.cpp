#include "UltraUtilities/Math/LatinSquare.h"
#include "UltraUtilities/Random.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Latin Squares", "[latin_square]")
{
	XorShiftRandom random;
	random.SetSeed(938532);

	SECTION("Test randomly generating latin squares.")
	{
		for (int i = 1; i < 10; i++)
		{
			LatinSquare latinSquare(i);
			latinSquare.RandomlyGenerate(random);
			REQUIRE(latinSquare.IsValid());
		}
	}

	SECTION("Test randomly generating sudoku square.")
	{
		SudokuSquare sudokuSquare;
		sudokuSquare.RandomlyGenerate(random);
		REQUIRE(sudokuSquare.IsValid());
	}
}