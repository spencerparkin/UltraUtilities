#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/DArray.hpp"

namespace UU
{
	/**
	 * This class can enumerate all combinations or permutations of any set of things
	 * so long as they can be zero-based indexed.
	 */
	class UU_API CombinatorialEnumerator
	{
	public:
		CombinatorialEnumerator();
		virtual ~CombinatorialEnumerator();

		/**
		 * Visit every possible way to choose the given number of choices from the given array of things
		 * where order does not matter.
		 */
		bool VisitAllCombinations(unsigned int numberOfChoices, DArray<unsigned int>& thingsArray);

		/**
		 * Visit every possible way to choose the given number of choices from the given array of things
		 * where order does matter.
		 */
		bool VisitAllPermutations(unsigned int numberOfChoices, DArray<unsigned int>& thingsArray);

		/**
		 * Visit all permutations of the given array of things.
		 */
		bool VisitAllPermutationsOf(DArray<unsigned int>& thingsArray);

		/**
		 * This gets called to visit each choices.  The user is, of course, expected to override it.
		 * 
		 * @return Return true to keep being called; false to terminate visitation.
		 */
		virtual bool VisitChoice(const DArray<unsigned int>& chosenThingsArray) = 0;
	};

	/**
	 * Provie a convenient way to use the enumerator with lambdas.
	 */
	template<typename L>
	class UU_API LambdaCombinatorialEnumerator : public CombinatorialEnumerator
	{
	public:
		LambdaCombinatorialEnumerator(L lambda) : lambda(lambda)
		{
		}

		virtual bool VisitChoice(const DArray<unsigned int>& chosenThingsArray) override
		{
			return this->lambda(chosenThingsArray);
		}

	private:
		L lambda;
	};
}