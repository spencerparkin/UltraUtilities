#pragma once

#include "UltraUtilities/Defines.h"

namespace UU
{
	/**
	 * This is the base class for any kind of psuedo-random number generator.
	 */
	class UU_API Random
	{
	public:
		Random();
		virtual ~Random();

		virtual void SetSeed(unsigned int seed);
		virtual unsigned int GetSeed();

		/**
		 * Return a random number in the given range, inclusive.
		 */
		virtual unsigned int GetRandomInteger(unsigned int minInteger, unsigned int maxInteger) = 0;

		/**
		 * Return a random true/false value.
		 */
		virtual bool CoinToss() = 0;

		/**
		 * Put the elements of the given array in a random order within the array.
		 */
		template<typename T>
		void Shuffle(T* array, unsigned int arraySize)
		{
			for (unsigned int i = 0; i < arraySize - 1; i++)
			{
				unsigned int j = this->GetRandomInteger(i, arraySize - 1);
				if (i != j)
				{
					T temp = array[i];
					array[i] = array[j];
					array[j] = temp;
				}
			}
		}

	protected:
		unsigned int seed;
	};

	/**
	 * See: https://en.wikipedia.org/wiki/Xorshift
	 */
	class UU_API XorShiftRandom : public Random
	{
	public:
		XorShiftRandom();
		virtual ~XorShiftRandom();

		virtual void SetSeed(unsigned int seed) override;
		virtual unsigned int GetRandomInteger(unsigned int minInteger, unsigned int maxInteger) override;
		virtual bool CoinToss() override;

	private:
		unsigned int NextRandom();

		unsigned int state;
	};
}