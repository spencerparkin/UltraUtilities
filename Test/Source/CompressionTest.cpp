#include "UltraUtilities/Compression/HuffmanCompression.h"
#include "UltraUtilities/Compression/LZ77Compression.h"
#include "UltraUtilities/Memory/Pointer.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Compression", "[compression]")
{
	SECTION("Simple compression/decompression tests.")
	{
		DArray<const char*> originalDataArray;

		// Courtesy of ChatGPT...
		originalDataArray.Push(
			"Lo, my lord, the silent, solemn scribe still sets slow letters upon the scroll,"
			"and still the scroll swells full with soft, low tales of love and loss."
			"Long, long he labors, letting little letters fall like light, like lull, like low,"
			"while all the hall lies hushed, and all the lamps lean low in loyal glow."
			"So shall the small, simple signs of his soul, set in subtle, solemn order,"
			"live longer than loud swords, and longer than lost kings,"
			"for silent script still sings when steel lies still and sleeping.");

		// Courtesy of ChatGPT...
		originalDataArray.Push(
			"Lo, fair moon, fair moon, that walk’st the night so still;"
			"Lo, fair moon, fair moon, that gild’st the quiet hill."
			"O gentle breeze, gentle breeze, that through the forest sighs;"
			"O gentle breeze, gentle breeze, that soft in darkness lies."
			"Hear me, sweet muse, sweet muse, that on the lyre dost dwell;"
			"Hear me, sweet muse, sweet muse, that weav’st the poet’s spell.");

		DArray<SharedPtr<Compression>> compressionArray;
		compressionArray.Push(SharedPtr<HuffmanCompression>::Make());
		compressionArray.Push(SharedPtr<LZ77Compression>::Make(64));

		for (unsigned int i = 0; i < compressionArray.GetSize(); i++)
		{
			Compression* compression = compressionArray[i].Get();

			const char* originalData = originalDataArray[i];
			int originalDataLength = ::strlen(originalData);
			MemoryBufferStream originalDataStream((char*)originalData, originalDataLength, true);

			RingBufferStream compressedDataStream(2048);

			bool compressed = compression->Compress(&originalDataStream, &compressedDataStream);
			REQUIRE(compressed);

			RingBufferStream decompressedDataStream(2024);

			bool decompressed = compression->Decompress(&compressedDataStream, &decompressedDataStream);
			REQUIRE(decompressed);

			REQUIRE(decompressedDataStream.GetSize() == originalDataLength);
			if (decompressedDataStream.GetSize() == originalDataLength)
			{
				char* decompressedData = new char[originalDataLength];
				decompressedDataStream.ReadBytes(decompressedData, originalDataLength);

				int compareResult = ::memcmp(originalData, decompressedData, originalDataLength);
				REQUIRE(compareResult == 0);

				delete[] decompressedData;
			}
		}
	}
}