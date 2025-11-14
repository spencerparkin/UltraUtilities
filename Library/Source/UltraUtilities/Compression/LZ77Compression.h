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
		unsigned int windowSize;

		struct Packet
		{
			unsigned short offset;
			unsigned short length;
			unsigned char byte;
		};
	};
}