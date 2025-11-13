#include "UltraUtilities/Compression/HuffmanCode.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Huffman", "[huffman]")
{
	SECTION("Simple compression/decompression test.")
	{
		const char* originalData = "This is a test of the Huffman compression scheme.  Let's see if it will work.  I'm really depressed, by the way.";
		int originalDataLength = ::strlen(originalData);

		MemoryBufferStream originalDataStream((char*)originalData, originalDataLength);

		RingBufferStream compressedDataStream(2048);

		bool compressed = HuffmanCode::Compress(&originalDataStream, &compressedDataStream);
		REQUIRE(compressed);

		RingBufferStream decompressedDataStream(2024);

		bool decompressed = HuffmanCode::Decompress(&compressedDataStream, &decompressedDataStream);
		REQUIRE(decompressed);

		// STPTODO: Make sure that the decompressed data matches the original data.
	}
}