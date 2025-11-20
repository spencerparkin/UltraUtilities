#include "UltraUtilities/Compression/LZ77Compression.h"
#include "UltraUtilities/Containers/DArray.hpp"
#include "UltraUtilities/Memory/BitStream.hpp"

using namespace UU;

LZ77Compression::LZ77Compression(unsigned int windowSize)
{
	this->windowSize = windowSize;
}

/*virtual*/ LZ77Compression::~LZ77Compression()
{
}

/*virtual*/ bool LZ77Compression::Compress(ByteStream* inputStream, ByteStream* outputStream)
{
	if (inputStream == outputStream)
		return false;

	const char* inputBuffer = inputStream->GetBuffer();
	if (!inputBuffer)
		return false;

	unsigned int inputBufferSize = inputStream->GetSize();
	if (inputBufferSize == 0)
		return false;

	BitStream outputBitStream(outputStream);
	if (!outputBitStream.WriteAllBits(inputBufferSize))
		return false;

	unsigned int numBits = this->CalcMaxBitsForOffsetOrLength();
	if (numBits >= sizeof(unsigned int) * 8)
		return false;

	unsigned int i;
	for (i = 0; i < windowSize; i++)
	{
		if (i >= inputBufferSize)
		{
			if (!outputBitStream.Flush())
				return false;

			return true;
		}

		if (!outputBitStream.WriteBits(char(0), 1))
			return false;

		if (!outputBitStream.WriteAllBits(inputBuffer[i]))
			return false;
	}

	while (i < inputBufferSize)
	{
		bool emitNextByte = true;

		unsigned int availableWindowSize = UU_MIN(windowSize, inputBufferSize - i);

		for (unsigned int j = 0; j < availableWindowSize; j++)
		{
			const char* patternBuffer = &inputBuffer[i];
			unsigned int patternBufferSize = availableWindowSize - j;	// Look for patterns from biggest to smallest.

			// If the pattern buffer isn't large enough, then it's not worth it.
			if (patternBufferSize < 3)
				break;

			const char* searchBuffer = &inputBuffer[i - windowSize];
			unsigned int searchBufferSize = windowSize;
			unsigned int foundPatternOffset = 0;

			if (this->FindPattern(patternBuffer, patternBufferSize, searchBuffer, searchBufferSize, foundPatternOffset))
			{
				if (!outputBitStream.WriteBits(char(1), 1))
					return false;

				unsigned int relativeOffset = searchBufferSize - foundPatternOffset;
				
				if (!outputBitStream.WriteBits(relativeOffset, numBits))
					return false;

				if (!outputBitStream.WriteBits(patternBufferSize, numBits))
					return false;

				i += patternBufferSize;
				emitNextByte = false;
				break;
			}
		}

		if (emitNextByte)
		{
			if (!outputBitStream.WriteBits(char(0), 1))
				return false;

			if (!outputBitStream.WriteAllBits(inputBuffer[i]))
				return false;

			i++;
		}
	}

	if (!outputBitStream.Flush())
		return false;

	return true;
}

bool LZ77Compression::FindPattern(const char* patternBuffer, unsigned int patternBufferSize, const char* searchBuffer, unsigned int searchBufferSize, unsigned int& foundPatternOffset)
{
	for (unsigned int i = 0; i < searchBufferSize; i++)
	{
		if (searchBufferSize - i < patternBufferSize)
			return false;

		unsigned int j;
		for (j = 0; j < patternBufferSize; j++)
			if (patternBuffer[j] != searchBuffer[i + j])
				break;

		if (j == patternBufferSize)
		{
			foundPatternOffset = i;
			return true;
		}
	}

	return false;
}

unsigned int LZ77Compression::CalcMaxBitsForOffsetOrLength()
{
	unsigned int numBits = 0;
	while ((1 << numBits) < this->windowSize)
		numBits++;

	return numBits;
}

/*virtual*/ bool LZ77Compression::Decompress(ByteStream* inputStream, ByteStream* outputStream)
{
	unsigned int numBits = this->CalcMaxBitsForOffsetOrLength();
	if (numBits >= sizeof(unsigned int) * 8)
		return false;

	BitStream inputBitStream(inputStream);

	unsigned int originalSize = 0;
	if (!inputBitStream.ReadAllBits(originalSize))
		return false;

	DArray<char> outputBuffer;
	outputBuffer.SetCapacity(originalSize);

	while (true)
	{
		char typeBit = 0;
		if (!inputBitStream.ReadBits(typeBit, 1))
			break;

		if (typeBit == 0)
		{
			char byte = 0;
			if (!inputBitStream.ReadAllBits(byte))
				return false;

			outputBuffer.Push(byte);
			if (outputBuffer.GetSize() == originalSize)
				break;
		}
		else if(typeBit == 1)
		{
			unsigned int relativeOffset = 0;
			if (!inputBitStream.ReadBits(relativeOffset, numBits))
				return false;

			unsigned int patternLength = 0;
			if (!inputBitStream.ReadBits(patternLength, numBits))
				return false;

			if (relativeOffset > outputBuffer.GetSize() || patternLength > relativeOffset)
				return false;		// Malformed data.

			unsigned int i = outputBuffer.GetSize() - relativeOffset;
			for (unsigned int j = 0; j < patternLength; j++)
				outputBuffer.Push(outputBuffer[i + j]);
		}
	}

	if (outputStream->WriteBytes(outputBuffer.GetBuffer(), outputBuffer.GetSize()) != outputBuffer.GetSize())
		return false;

	return true;
}