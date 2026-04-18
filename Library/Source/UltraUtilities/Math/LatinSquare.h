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

		/**
		 * Here we employ an algorithm very similar to the one that generates a
		 * random square, but here the idea is to complete the square.  All entries
		 * should be -1 except for a few sparse entries.  If there is just one solution
		 * to the square, then we should find it.  If there are many, then we don't
		 * define here which we find, but we should find one of them.  If there is
		 * no solution, then we should return false here, and the square should remain
		 * untouched.
		 */
		bool CompleteSquare(int* numBacktracks = nullptr);

		virtual LatinSquare* Clone() const;

		bool SetValue(int row, int col, int value);
		bool GetValue(int row, int col, int& value) const;
		bool CoordsValid(int row, int col) const;
		int GetSize() const;
		void Clear();
		bool Copy(const LatinSquare* latinSquare);

	protected:
		/**
		 * It's important to understand here that only the values prior to this
		 * in a book-reading sense (top to bottom, left to right) are valid
		 * values that can be read from the matrix when making the determination.
		 */
		virtual bool CanPlaceValueAtTargetLocation(int targetRow, int targetCol, int value);

		/**
		 * This is used by the solver.  You can read any value of the matrix,
		 * but should ignore any values of -1.
		 */
		virtual void GetAllPossibleValuesForLocation(int targetRow, int targetCol, DArray<int>& possibleValuesArray);

		/**
		 * This is used by the @ref GetAllPossibleValuesForLocation function.
		 */
		virtual void BumpIllegalValuesForLocation(int targetRow, int targetCol, DArray<int>& countArray);

		bool IsPermutation(const int* permutationArray) const;
		bool RandomlyGenerateInternal(Random& random, int targetRow, int targetCol);

		int size;
		int** matrix;
	};
}