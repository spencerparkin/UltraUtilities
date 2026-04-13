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
	DArray<int> valueArray;
	for (int i = 0; i < this->size * this->size; i++)
		valueArray.Push(i % this->size);

	random.Shuffle(valueArray.GetBuffer(), valueArray.GetSize());

	List<int> valueQueue;
	for (int i = 0; i < this->size * this->size; i++)
		valueQueue.PushBack(valueArray[i]);

	for (int row = 0; row < this->size; row++)
		for (int col = 0; col < this->size; col++)
			this->matrix[row][col] = -1;

	struct Location
	{
		int row;
		int col;
	};

	DArray<Location> vacantLocationArray;
	for (int row = 0; row < this->size; row++)
		for (int col = 0; col < this->size; col++)
			vacantLocationArray.Push(Location{ row, col });

	DArray<Location> usedLocationArray;

	random.Shuffle(vacantLocationArray.GetBuffer(), vacantLocationArray.GetSize());

	// This isn't a very efficient algorithm, and I can't prove that it will always terminate.
	while (valueQueue.GetNumValues() > 0)
	{
		int value = -1;
		valueQueue.PopFront(&value);

		bool valuePlaced = false;

		for (int i = 0; i < (int)vacantLocationArray.GetSize(); i++)
		{
			const Location& location = vacantLocationArray[i];

			if (this->CanLegallySetValue(location.row, location.col, value))
			{
				this->matrix[location.row][location.col] = value;
				vacantLocationArray.QuickRemove(i);
				usedLocationArray.Push(location);
				valuePlaced = true;
				break;
			}
		}

		if (valuePlaced)
			continue;

		valueQueue.PushBack(value);

		// Here, a value that we must place could not be placed in any vacant location.
		// Therefore, our only hope of placing it requires us to open up a new vacancy.
		// We'll do this by moving an already-placed value into an existing vacancy, which
		// should always be possible to do.

		random.Shuffle(usedLocationArray.GetBuffer(), usedLocationArray.GetSize());
		random.Shuffle(vacantLocationArray.GetBuffer(), vacantLocationArray.GetSize());

		bool valueSwapped = false;

		for (int i = 0; i < (int)usedLocationArray.GetSize(); i++)
		{
			const Location& usedLocation = usedLocationArray[i];
			value = this->matrix[usedLocation.row][usedLocation.col];
			this->matrix[usedLocation.row][usedLocation.col] = -1;

			for (int j = 0; j < (int)vacantLocationArray.GetSize(); j++)
			{
				const Location& vacantLocation = vacantLocationArray[j];
				if (this->CanLegallySetValue(vacantLocation.row, vacantLocation.col, value))
				{
					this->matrix[vacantLocation.row][vacantLocation.col] = value;
					usedLocationArray.Push(vacantLocation);
					vacantLocationArray.Push(usedLocation);
					valueSwapped = true;
					break;
				}
			}

			if (valueSwapped)
				break;

			this->matrix[usedLocation.row][usedLocation.col] = value;
		}

		UU_ASSERT(valueSwapped);
	}
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

bool LatinSquare::CanLegallySetValue(int row, int col, int value) const
{
	if (!this->CoordsValid(row, col))
		return false;

	for (int i = 0; i < this->size; i++)
	{
		if (i == col)
			continue;

		if (this->matrix[row][i] == value)
			return false;
	}

	for (int i = 0; i < this->size; i++)
	{
		if (i == row)
			continue;

		if (this->matrix[i][col] == value)
			return false;
	}

	return true;
}