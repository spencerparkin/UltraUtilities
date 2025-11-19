#pragma once

#include "UltraUtilities/Compression/Compression.h"
#include "UltraUtilities/Containers/DArray.hpp"
#include "UltraUtilities/Containers/HashMap.hpp"
#include "UltraUtilities/Memory/BitStream.hpp"

namespace UU
{
	/**
	 * This class implements the Huffman encoding/decoding scheme.
	 */
	class UU_API HuffmanCompression : public Compression
	{
	public:
		HuffmanCompression();
		virtual ~HuffmanCompression();

		virtual bool Compress(ByteStream* inputStream, ByteStream* outputStream) override;
		virtual bool Decompress(ByteStream* inputStream, ByteStream* outputStream) override;

		class Node
		{
		public:
			Node();
			virtual ~Node();

			bool Serialize(BitStream* bitStream) const;
			static Node* Deserialize(BitStream* bitStream);

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