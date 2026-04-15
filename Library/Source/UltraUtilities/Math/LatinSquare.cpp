#include "UltraUtilities/Math/LatinSquare.h"
#include "UltraUtilities/Random.h"
#include "UltraUtilities/Containers/DArray.hpp"

using namespace UU;

//-------------------------- LatinSquare --------------------------

LatinSquare::LatinSquare(int size)
{
	UU_ASSERT(size > 0);

	this->size = size;
	this->matrix = new int* [this->size];
	for (int row = 0; row < this->size; row++)
	{
		this->matrix[row] = new int[this->size];
		for (int col = 0; col < this->size; col++)
			this->matrix[row][col] = 0;
	}
}

/*virtual*/ LatinSquare::~LatinSquare()
{
	for (int row = 0; row < this->size; row++)
		delete[] this->matrix[row];
	delete[] this->matrix;
}

int LatinSquare::GetSize() const
{
	return this->size;
}

/*virtual*/ bool LatinSquare::IsValid() const
{
	for (int row = 0; row < this->size; row++)
		if (!this->IsPermutation(this->matrix[row]))
			return false;

	DArray<int> colArray(this->size);

	for (int col = 0; col < this->size; col++)
	{
		for (int row = 0; row < this->size; row++)
			colArray[row] = this->matrix[row][col];

		if (!this->IsPermutation(colArray.GetBuffer()))
			return false;
	}

	return true;
}

bool LatinSquare::IsPermutation(const int* permutationArray) const
{
	DArray<int> countArray(this->size);
	for (int i = 0; i < this->size; i++)
		countArray[i] = 0;

	for (int i = 0; i < this->size; i++)
	{
		int j = permutationArray[i];

		if (j < 0 || j >= this->size)
			return false;

		if (countArray[j] != 0)
			return false;

		countArray[j]++;
	}

	return true;
}

void LatinSquare::RandomlyGenerate(Random& random)
{
	for (int row = 0; row < this->size; row++)
		for (int col = 0; col < this->size; col++)
			this->matrix[row][col] = -1;

	bool success = this->RandomlyGenerateInternal(random, 0, 0);
	UU_ASSERT(success);
}

 /*virtual*/ bool LatinSquare::CanPlaceValueAtTargetLocation(int targetRow, int targetCol, int value)
{
	 for (int col = 0; col < targetCol; col++)
		 if (this->matrix[targetRow][col] == value)
			 return false;

	 for (int row = 0; row < targetRow; row++)
		 if (this->matrix[row][targetCol] == value)
			 return false;

	 return true;
}

bool LatinSquare::RandomlyGenerateInternal(Random& random, int targetRow, int targetCol)
{
	DArray<int> possibleValuesArray;

	for (int i = 0; i < this->size; i++)
		if (this->CanPlaceValueAtTargetLocation(targetRow, targetCol, i))
			possibleValuesArray.Push(i);

	if (possibleValuesArray.GetSize() == 0)
		return false;

	random.Shuffle(possibleValuesArray.GetBuffer(), possibleValuesArray.GetSize());

	int nextTargetRow = targetRow;
	int nextTargetCol = targetCol;

	if (++nextTargetCol == this->size)
	{
		nextTargetCol = 0;
		nextTargetRow++;
	}

	for (int i = 0; i < (int)possibleValuesArray.GetSize(); i++)
	{
		this->matrix[targetRow][targetCol] = possibleValuesArray[i];

		if (nextTargetRow == this->size)
			return true;

		if (this->RandomlyGenerateInternal(random, nextTargetRow, nextTargetCol))
			return true;
	}

	return false;
}

bool LatinSquare::SetValue(int row, int col, int value)
{
	if (!this->CoordsValid(row, col))
		return false;

	this->matrix[row][col] = value;
	return true;
}

bool LatinSquare::GetValue(int row, int col, int& value) const
{
	if (!this->CoordsValid(row, col))
		return false;

	value = this->matrix[row][col];
	return true;
}

bool LatinSquare::CoordsValid(int row, int col) const
{
	if (!(0 <= row && row < this->size))
		return false;

	if (!(0 <= col && col < this->size))
		return false;

	return true;
}

//-------------------------- SudokuSquare --------------------------

SudokuSquare::SudokuSquare() : LatinSquare(9)
{
}

/*virtual*/ SudokuSquare::~SudokuSquare()
{
}

/*virtual*/ bool SudokuSquare::CanPlaceValueAtTargetLocation(int targetRow, int targetCol, int value)
{
	if (!LatinSquare::CanPlaceValueAtTargetLocation(targetRow, targetCol, value))
		return false;

	int subSquareRow = 3 * (targetRow / 3);
	int subSquareCol = 3 * (targetCol / 3);

	for (int row = subSquareRow; row <= targetRow; row++)
	{
		int colStop = (row == targetRow) ? targetCol : (subSquareCol + 3);
		for (int col = subSquareCol; col < colStop; col++)
			if (this->matrix[row][col] == value)
				return false;
	}

	return true;
}

/*virtual*/ bool SudokuSquare::IsValid() const
{
	if (!LatinSquare::IsValid())
		return false;

	DArray<int> valuesCountArray;
	valuesCountArray.SetSize(9);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 9; k++)
				valuesCountArray[k] = 0;

			for (int row = i * 3; row < (i + 1) * 3; row++)
			{
				for (int col = j * 3; col < (j + 1) * 3; col++)
				{
					int value = this->matrix[row][col];

					if (valuesCountArray[value] != 0)
						return false;

					valuesCountArray[value]++;
				}
			}
		}
	}

	return true;
}