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

	private:
		unsigned int NextRandom();

		unsigned int state;
	};
}