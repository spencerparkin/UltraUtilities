#include "UltraUtilities/Math/LatinSquare.h"
#include "UltraUtilities/Random.h"
#include "UltraUtilities/Containers/List.hpp"
#include "UltraUtilities/Containers/DArray.hpp"

using namespace UU;

LatinSquare::LatinSquare(int size)
{
	this->size = size;
	this->matrix = new int* [this->size];
	for (int row = 0; row < this->size; row++)
	{
		this->matrix[row] = new int[this->size];
		for (int col = 0; col < this->size; col++)
			this->matrix[col] = 0;
	}
}

/*virtual*/ LatinSquare::~LatinSquare()
{
	for (int row = 0; row < this->size; row++)
		delete[] this->matrix[row];
	delete[] this->matrix;
}

bool LatinSquare::IsValid() const
{
	for (int row = 0; row < this->size; row++)
		if (!this->IsPermutation(this->matrix[row]))
			return false;

	bool isValid = true;
	int* colArray = new int[this->size];

	for (int col = 0; col < this->size; col++)
	{
		for (int row = 0; row < this->size; row++)
			colArray[row] = this->matrix[row][col];

		if (!this->IsPermutation(colArray))
		{
			isValid = false;
			break;
		}
	}

	delete[] colArray;

	return isValid;
}

bool LatinSquare::IsPermutation(int* permutationArray) const
{
	int* countArray = new int[this->size];
	for (int i = 0; i < this->size; i++)
		countArray[i] = 0;

	bool isValid = true;

	for (int i = 0; i < this->size; i++)
	{
		int j = permutationArray[i];

		if (j < 0 || j >= this->size)
		{
			isValid = false;
			break;
		}

		if (countArray[j] != 0)
		{
			isValid = false;
			break;
		}

		countArray[j]++;
	}

	delete[] countArray;

	return isValid;
}

void LatinSquare::RandomlyGenerate(Random& random)
{
	
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