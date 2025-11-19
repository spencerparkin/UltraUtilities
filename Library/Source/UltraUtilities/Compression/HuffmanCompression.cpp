#include "UltraUtilities/Compression/HuffmanCompression.h"
#include "UltraUtilities/Containers/RBMap.hpp"
#include "UltraUtilities/Containers/PriorityQueue.hpp"
#include "UltraUtilities/Memory/Pointer.hpp"

using namespace UU;

//----------------------------- HuffmanCompression -----------------------------

HuffmanCompression::HuffmanCompression()
{
}

/*virtual*/ HuffmanCompression::~HuffmanCompression()
{
}

/*virtual*/ bool HuffmanCompression::Compress(ByteStream* inputStream, ByteStream* outputStream)
{
	if (inputStream == outputStream)
		return false;

	const char* inputBuffer = inputStream->GetBuffer();
	if (!inputBuffer)
		return false;

	RBMap<char, Node*> alphabetMap;

	for (unsigned int i = 0; i < inputStream->GetSize(); i++)
	{
		char character = inputBuffer[i];

		Node* node = nullptr;
		if (alphabetMap.Find(character, &node))
			node->frequency++;
		else
		{
			node = new Node();
			node->character = character;
			node->type = Node::Type::LEAF;
			node->frequency = 1;
			alphabetMap.Insert(character, node);
		}
	}

	if (alphabetMap.GetNumPairs() == 0)
		return false;

	StaticPriorityQueue<Node*> nodeQueue;
	for (auto pair : alphabetMap)
		nodeQueue.InsertKey(pair.value);

	while (nodeQueue.GetSize() >= 2)
	{
		Node* nodeA = nullptr;
		nodeQueue.RemoveHighestPriorityKey(nodeA);

		Node* nodeB = nullptr;
		nodeQueue.RemoveHighestPriorityKey(nodeB);

		Node* nodeC = new Node();
		nodeC->type = Node::Type::INTERNAL;
		nodeC->frequency = nodeA->frequency + nodeB->frequency;
		nodeC->node[0] = nodeA;
		nodeC->node[1] = nodeB;

		nodeQueue.InsertKey(nodeC);
	}

	Node* rootNode = nullptr;
	nodeQueue.RemoveHighestPriorityKey(rootNode);
	
	UniquePtr<Node> nodeTree(rootNode);

	BitStream outputBitStream(outputStream);

	if (!rootNode->Serialize(&outputBitStream))
		return false;

	HashMap<char, DArray<char>> codeMap;
	DArray<char> treePath;
	rootNode->PopulateCodeMap(codeMap, treePath);

	for (int i = 0; i < inputStream->GetSize(); i++)
	{
		char character = inputBuffer[i];
		DArray<char>* charTreePath = nullptr;
		codeMap.FindPtr(character, charTreePath);

		for (int j = 0; j < charTreePath->GetSize(); j++)
		{
			char pathBit = (*charTreePath)[j];
			if (!outputBitStream.WriteBits(pathBit, 1))
				return false;
		}
	}

	if (!outputBitStream.Flush())
		return false;

	return true;
}

/*virtual*/ bool HuffmanCompression::Decompress(ByteStream* inputStream, ByteStream* outputStream)
{
	if (inputStream == outputStream)
		return false;

	BitStream inputBitStream(inputStream);

	UniquePtr<Node> rootNode(Node::Deserialize(&inputBitStream));
	if (!rootNode.Get())
		return false;

	Node* node = rootNode;

	while (true)
	{
		if (node->type == Node::Type::LEAF)
		{
			if (!inputBitStream.ReadAllBits(node->character))
				return false;

			node = rootNode;
		}
		else if (node->type == Node::Type::INTERNAL)
		{
			char pathBit = 0;
			if (!inputBitStream.ReadBits(pathBit, 1))
				return false;

			node = node->node[pathBit];
		}
		else
		{
			return false;
		}
	}

	return true;
}

//----------------------------- HuffmanCompression::Node -----------------------------

HuffmanCompression::Node::Node()
{
	this->frequency = 0;
	this->type = Type::LEAF;
	this->character = '\0';
}

/*virtual*/ HuffmanCompression::Node::~Node()
{
	if (this->type == Type::INTERNAL)
	{
		delete this->node[0];
		delete this->node[1];
	}
}

bool HuffmanCompression::Node::Serialize(BitStream* bitStream) const
{
	bitStream->WriteBits(this->type, 1);

	switch (this->type)
	{
		case Type::INTERNAL:
		{
			if (!this->node[0]->Serialize(bitStream))
				return false;

			if (!this->node[1]->Serialize(bitStream))
				return false;

			break;
		}
		case Type::LEAF:
		{
			if (bitStream->WriteAllBits(this->character))
				return false;

			break;
		}
	}

	return true;
}

/*static*/ HuffmanCompression::Node* HuffmanCompression::Node::Deserialize(BitStream* bitStream)
{
	UniquePtr<Node> node(new Node());

	if (!bitStream->ReadBits(*(reinterpret_cast<char*>(&node->type)), 1))
		return nullptr;

	switch (node->type)
	{
		case Type::INTERNAL:
		{
			node->node[0] = Node::Deserialize(bitStream);
			if (!node->node[0])
				return nullptr;

			node->node[1] = Node::Deserialize(bitStream);
			if (!node->node[1])
				return nullptr;

			break;
		}
		case Type::LEAF:
		{
			if (!bitStream->ReadAllBits(node->character))
				return nullptr;

			break;
		}
	}

	return node.Release();
}

void HuffmanCompression::Node::PopulateCodeMap(HashMap<char, DArray<char>>& codeMap, DArray<char>& treePath) const
{
	switch (this->type)
	{
		case Type::INTERNAL:
		{
			treePath.Push(0);
			this->node[0]->PopulateCodeMap(codeMap, treePath);
			treePath.Pop();

			treePath.Push(1);
			this->node[1]->PopulateCodeMap(codeMap, treePath);
			treePath.Pop();

			break;
		}
		case Type::LEAF:
		{
			codeMap.Insert(this->character, treePath);
			break;
		}
	}
}