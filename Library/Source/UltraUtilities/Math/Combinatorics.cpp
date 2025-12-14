#include "UltraUtilities/Math/Combinatorics.h"

using namespace UU;

CombinatorialEnumerator::CombinatorialEnumerator()
{
}

/*virtual*/ CombinatorialEnumerator::~CombinatorialEnumerator()
{
}

bool CombinatorialEnumerator::VisitAllCombinations(unsigned int numberOfChoices, DArray<unsigned int>& thingsArray)
{
	if (numberOfChoices > thingsArray.GetSize())
		return false;

	if (numberOfChoices == thingsArray.GetSize())
		return this->VisitChoice(thingsArray);
	
	unsigned int thing = thingsArray.Pop((unsigned int)0);

	LambdaCombinatorialEnumerator enumeratorA([this, thing](const DArray<unsigned int>& subChosenThingsArray) -> bool
		{
			DArray<unsigned int> otherThingsArray;
			otherThingsArray.SetCapacity(subChosenThingsArray.GetSize() + 1);
			otherThingsArray.Push(thing);
			for (unsigned int i = 0; i < subChosenThingsArray.GetSize(); i++)
				otherThingsArray.Push(subChosenThingsArray[i]);
			
			return this->VisitChoice(otherThingsArray);
		});

	if (!enumeratorA.VisitAllCombinations(numberOfChoices - 1, thingsArray))
		return false;

	LambdaCombinatorialEnumerator enumeratorB([this](const DArray<unsigned int>& subChosenThingsArray) -> bool
		{
			return this->VisitChoice(subChosenThingsArray);
		});

	if (!enumeratorB.VisitAllCombinations(numberOfChoices, thingsArray))
		return false;

	return true;
}

bool CombinatorialEnumerator::VisitAllPermutations(unsigned int numberOfChoices, DArray<unsigned int>& thingsArray)
{
	LambdaCombinatorialEnumerator enumerator([this](const DArray<unsigned int>& chosenThingsArray) -> bool
		{
			DArray<unsigned int> chosenThingsCopy = chosenThingsArray;
			return this->VisitAllPermutationsOf(chosenThingsCopy);
		});

	return enumerator.VisitAllCombinations(numberOfChoices, thingsArray);
}

bool CombinatorialEnumerator::VisitAllPermutationsOf(DArray<unsigned int>& thingsArray)
{
	if (thingsArray.GetSize() == 1)
		return this->VisitChoice(thingsArray);

	unsigned int thing = thingsArray.Pop();

	LambdaCombinatorialEnumerator enumerator([this, thing](const DArray<unsigned int>& subChosenThingsArray) -> bool
		{
			DArray<unsigned int> chosenThingsArray;
			chosenThingsArray.SetCapacity(subChosenThingsArray.GetSize() + 1);

			for (unsigned int i = 0; i < chosenThingsArray.GetCapacity(); i++)
			{
				chosenThingsArray.SetSize(0);
				unsigned int k = 0;

				for (unsigned int j = 0; j < chosenThingsArray.GetSize(); j++)
				{
					if (i == j)
						chosenThingsArray.Push(thing);
					else
						chosenThingsArray.Push(subChosenThingsArray[k++]);
				}

				if (!this->VisitChoice(chosenThingsArray))
					return false;
			}

			return true;
		});

	return enumerator.VisitAllPermutationsOf(thingsArray);
}