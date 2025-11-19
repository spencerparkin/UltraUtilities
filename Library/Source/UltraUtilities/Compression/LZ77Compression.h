#pragma once

#include "UltraUtilities/Compression/Compression.h"

namespace UU
{
	/**
	 * This class implements the Lempel/Ziv 1977 encoding/decoding scheme.
	 */
	class UU_API LZ77Compression : public Compression
	{
	public:
		LZ77Compression(unsigned int windowSize);
		virtual ~LZ77Compression();

		virtual bool Compress(ByteStream* inputStream, ByteStream* outputStream) override;
		virtual bool Decompress(ByteStream* inputStream, ByteStream* outputStream) override;

	private:
		bool FindPattern(const char* patternBuffer, unsigned int patternBufferSize, const char* searchBuffer, unsigned int searchBufferSize, unsigned int& foundPatternOffset);

		unsigned int CalcMaxBitsForOffsetOrLength();

		unsigned int windowSize;
	};
}