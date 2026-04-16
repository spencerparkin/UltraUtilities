#include "UltraUtilities/Math/LatinSquare.h"
#include "UltraUtilities/Random.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

static void PrintSquare(const LatinSquare* latinSquare)
{
	for (int row = 0; row < latinSquare->GetSize(); row++)
	{
		for (int col = 0; col < latinSquare->GetSize(); col++)
		{
			int value = -1;
			latinSquare->GetValue(row, col, value);
			if (value < 0 || value >= latinSquare->GetSize())
				printf("*");
			else
				printf("%d", value);
			if (col < latinSquare->GetSize() - 1)
				printf(", ");
		}

		printf("\n");
	}

	printf("\n");
}

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
			PrintSquare(&latinSquare);
			REQUIRE(latinSquare.IsValid());
		}
	}

	SECTION("Test randomly generating sudoku square.")
	{
		SudokuSquare sudokuSquare;
		sudokuSquare.RandomlyGenerate(random);
		PrintSquare(&sudokuSquare);
		REQUIRE(sudokuSquare.IsValid());
	}

	SECTION("Solve a sudoku square.")
	{
		SudokuSquare sudokuSquare;

		// Found this puzzle on https://en.wikipedia.org/wiki/Sudoku
		sudokuSquare.SetValue(0, 0, 4);
		sudokuSquare.SetValue(0, 1, 2);
		sudokuSquare.SetValue(0, 4, 6);
		sudokuSquare.SetValue(1, 0, 5);
		sudokuSquare.SetValue(1, 3, 0);
		sudokuSquare.SetValue(1, 4, 8);
		sudokuSquare.SetValue(1, 5, 4);
		sudokuSquare.SetValue(2, 1, 8);
		sudokuSquare.SetValue(2, 2, 7);
		sudokuSquare.SetValue(2, 7, 5);
		sudokuSquare.SetValue(3, 0, 7);
		sudokuSquare.SetValue(3, 4, 5);
		sudokuSquare.SetValue(3, 8, 2);
		sudokuSquare.SetValue(4, 0, 3);
		sudokuSquare.SetValue(4, 3, 7);
		sudokuSquare.SetValue(4, 5, 2);
		sudokuSquare.SetValue(4, 8, 0);
		sudokuSquare.SetValue(5, 0, 6);
		sudokuSquare.SetValue(5, 4, 1);
		sudokuSquare.SetValue(5, 8, 5);
		sudokuSquare.SetValue(6, 1, 5);
		sudokuSquare.SetValue(6, 6, 1);
		sudokuSquare.SetValue(6, 7, 7);
		sudokuSquare.SetValue(7, 3, 3);
		sudokuSquare.SetValue(7, 4, 0);
		sudokuSquare.SetValue(7, 5, 8);
		sudokuSquare.SetValue(7, 8, 4);
		sudokuSquare.SetValue(8, 4, 7);
		sudokuSquare.SetValue(8, 7, 6);
		sudokuSquare.SetValue(8, 8, 8);

		PrintSquare(&sudokuSquare);

		bool completed = sudokuSquare.CompleteSquare();
		REQUIRE(completed);

		PrintSquare(&sudokuSquare);

		bool valid = sudokuSquare.IsValid();
		REQUIRE(valid);
	}
}