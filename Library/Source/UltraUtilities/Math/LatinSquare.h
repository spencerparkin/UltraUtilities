#pragma once

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
		bool IsValid() const;

		/**
		 * Assign a latin square to this matrix at random.  All possible latin
		 * squares should be equally likely to be chosen here.
		 */
		void RandomlyGenerate(Random& random);

		bool SetValue(int row, int col, int value);
		bool GetValue(int row, int col, int& value) const;
		bool CoordsValid(int row, int col) const;
		int GetSize() const;

	private:
		bool IsPermutation(const int* permutationArray) const;
		bool RandomlyGenerateInternal(Random& random, int targetRow, int targetCol);

		int size;
		int** matrix;
	};
}