#include "UltraUtilities/Random.h"

using namespace UU;

//---------------------------------- Random ----------------------------------

Random::Random()
{
	this->seed = 0;
}

/*virtual*/ Random::~Random()
{
}

/*virtual*/ void Random::SetSeed(unsigned int seed)
{
	this->seed = seed;
}

/*virtual*/ unsigned int Random::GetSeed()
{
	return this->seed;
}

//---------------------------------- XorShiftRandom ----------------------------------

XorShiftRandom::XorShiftRandom()
{
	this->state = 0;
}

/*virtual*/ XorShiftRandom::~XorShiftRandom()
{
}

/*virtual*/ void XorShiftRandom::SetSeed(unsigned int seed)
{
	Random::SetSeed(seed);

	this->state = seed;
}

/*virtual*/ unsigned int XorShiftRandom::GetRandomInteger(unsigned int minInteger, unsigned int maxInteger)
{
	unsigned int randomInteger = this->NextRandom();
	double alpha = double(randomInteger) / 4294967295.0;
	double randomFloat = double(minInteger) + alpha * double(maxInteger - minInteger);
	randomInteger = (unsigned int)randomFloat;
	randomInteger = UU_MAX(randomInteger, minInteger);
	randomInteger = UU_MIN(randomInteger, maxInteger);
	return randomInteger;
}

/*virtual*/ bool XorShiftRandom::CoinToss()
{
	unsigned int randomInteger = this->NextRandom();
	return randomInteger > 2147483647;
}

unsigned int XorShiftRandom::NextRandom()
{
	this->state ^= this->state << 13;
	this->state ^= this->state >> 17;
	this->state ^= this->state << 5;

	return this->state;
}