#pragma once

#include "UltraUtilities/Containers/DArray.hpp"

namespace UU
{
	class Random;

	/**
	 * A latin square is an NxN matrix where only the numbers 0 through N-1
	 * appear in the matrix and each such number appears exactly once in
	 * every row and column.  Given a latin square, the exchange of any two
	 * rows or columns gives another latin square, but not every latin square
	 * can be reached from any other latin square by a sequence of such
	 * exchanges.
	 */
	class LatinSquare
	{
	public:
		LatinSquare(int size);
		virtual ~LatinSquare();

		/**
		 * Tell us if this square matrix is a valid latin square.
		 */
		virtual bool IsValid() const;

		/**
		 * Assign a latin square to this matrix at random.  All possible latin
		 * squares should be equally likely to be chosen here.
		 */
		void RandomlyGenerate(Random& random);

		bool SetValue(int row, int col, int value);
		bool GetValue(int row, int col, int& value) const;
		bool CoordsValid(int row, int col) const;
		int GetSize() const;

	protected:
		/**
		 * It's important to understand here that only the values prior to this
		 * in a book-reading sense (top to bottom, left to right) are valid
		 * values that can be read from the matrix when making the determination.
		 */
		virtual bool CanPlaceValueAtTargetLocation(int targetRow, int targetCol, int value);

		bool IsPermutation(const int* permutationArray) const;
		bool RandomlyGenerateInternal(Random& random, int targetRow, int targetCol);

		int size;
		int** matrix;
	};

	/**
	 * This is a special case of 9x9 latin square where
	 * each 3x3 sub-square also contains the numberse 0-9.
	 */
	class SudokuSquare : public LatinSquare
	{
	public:
		SudokuSquare();
		virtual ~SudokuSquare();

		virtual bool IsValid() const override;

	protected:
		virtual bool CanPlaceValueAtTargetLocation(int targetRow, int targetCol, int value) override;
	};
}