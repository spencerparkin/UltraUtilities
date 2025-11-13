#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/DArray.hpp"
#include "UltraUtilities/Containers/HashMap.hpp"
#include "UltraUtilities/Memory/ByteStream.h"

namespace UU
{
	class UU_API HuffmanCode
	{
	public:
		/**
		 * Encode the given input stream into the given output stream using the Huffman encoding scheme.
		 * 
		 * @param[in] inputStream This is the data to compress.
		 * @param[out] outputStream This will hold the compression result.
		 * @return True is returned on success; false, otherwise.
		 */
		static bool Compress(ByteStream* inputStream, ByteStream* outputStream);

		/**
		 * Decode the given input stream into the given output stream using the Huffman decoding scheme.
		 * 
		 * @param[in] inputStream This is the data to decompress.
		 * @param[out] outputStream This will hold the decompression result.
		 * @return True is returned on success; false, otherwise.
		 */
		static bool Decompress(ByteStream* inputStream, ByteStream* outputStream);

		class Node
		{
		public:
			Node();
			virtual ~Node();

			bool Serialize(ByteStream* byteStream) const;
			static Node* Deserialize(ByteStream* byteStream);

			void PopulateCodeMap(HashMap<char, DArray<char>>& codeMap, DArray<char>& treePath) const;

			unsigned int frequency;

			bool operator<(const Node* node) const
			{
				return this->frequency < node->frequency;
			}

			enum Type : char
			{
				INTERNAL,
				LEAF
			};

			Type type;

			union
			{
				Node* node[2];
				char character;
			};
		};
	};
}