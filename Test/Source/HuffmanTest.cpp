#include "UltraUtilities/Compression/HuffmanCode.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Huffman", "[huffman]")
{
	SECTION("Simple compression/decompression test.")
	{
		// Courtesy of ChatGPT...
		const char* originalData =
			"Lo, my lord, the silent, solemn scribe still sets slow letters upon the scroll,"
			"and still the scroll swells full with soft, low tales of love and loss."
			"Long, long he labors, letting little letters fall like light, like lull, like low,"
			"while all the hall lies hushed, and all the lamps lean low in loyal glow."
			"So shall the small, simple signs of his soul, set in subtle, solemn order,"
			"live longer than loud swords, and longer than lost kings,"
			"for silent script still sings when steel lies still and sleeping.";

		int originalDataLength = ::strlen(originalData);

		MemoryBufferStream originalDataStream((char*)originalData, originalDataLength, true);

		RingBufferStream compressedDataStream(2048);

		bool compressed = HuffmanCode::Compress(&originalDataStream, &compressedDataStream);
		REQUIRE(compressed);

		RingBufferStream decompressedDataStream(2024);

		bool decompressed = HuffmanCode::Decompress(&compressedDataStream, &decompressedDataStream);
		REQUIRE(decompressed);

		// STPTODO: Make sure that the decompressed data matches the original data.
	}
}