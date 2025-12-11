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
	// STPTODO: Write this.  Could we base it on the VisitAllCombinations algorithm?

	return false;
}