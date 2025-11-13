#include "UltraUtilities/Compression/HuffmanCode.h"
#include "UltraUtilities/Containers/RBMap.hpp"
#include "UltraUtilities/Containers/PriorityQueue.hpp"
#include "UltraUtilities/Memory/Pointer.hpp"

using namespace UU;

//----------------------------- HuffmanCode -----------------------------

/*static*/ bool HuffmanCode::Compress(ByteStream* inputStream, ByteStream* outputStream)
{
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

	if (!rootNode->Serialize(outputStream))
		return false;

	HashMap<char, DArray<char>> codeMap;
	DArray<char> treePath;
	rootNode->PopulateCodeMap(codeMap, treePath);

	unsigned int bitOffset = 0;
	char byte = 0;

	for (int i = 0; i < inputStream->GetSize(); i++)
	{
		char character = inputBuffer[i];
		DArray<char>* charTreePath = nullptr;
		codeMap.Find(character, charTreePath);

		for (int j = 0; j < charTreePath->GetSize(); j++)
		{
			if (bitOffset == 8)
			{
				bitOffset = 0;
				if (outputStream->WriteBytes(&byte, 1) != 1)
					return false;

				byte = 0;
			}

			if ((*charTreePath)[j] != 0)
				byte |= (1 << bitOffset);

			bitOffset++;
		}
	}

	if (bitOffset != 0)
	{
		if (outputStream->WriteBytes(&byte, 1) != 1)
			return false;
	}

	return true;
}

/*static*/ bool HuffmanCode::Decompress(ByteStream* inputStream, ByteStream* outputStream)
{
	UniquePtr<Node> rootNode(Node::Deserialize(inputStream));
	if (!rootNode.Get())
		return false;

	char byte = 0;
	if (inputStream->ReadBytes(&byte, 1) != 1)
		return false;

	unsigned int bitOffset = 0;
	Node* node = rootNode;

	while (true)
	{
		if (node->type == Node::Type::LEAF)
		{
			if (outputStream->WriteBytes(&node->character, 1) != 1)
				return false;

			node = rootNode;
		}
		else if (node->type == Node::Type::INTERNAL)
		{
			if ((byte & (1 << bitOffset)) == 0)
				node = node->node[0];
			else
				node = node->node[1];

			if (++bitOffset == 8)
			{
				bitOffset = 0;
				if (inputStream->ReadBytes(&byte, 1) != 1)
					break;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

//----------------------------- HuffmanCode::Node -----------------------------

HuffmanCode::Node::Node()
{
	this->frequency = 0;
	this->type = Type::LEAF;
	this->character = '\0';
}

/*virtual*/ HuffmanCode::Node::~Node()
{
	if (this->type == Type::INTERNAL)
	{
		delete this->node[0];
		delete this->node[1];
	}
}

bool HuffmanCode::Node::Serialize(ByteStream* byteStream) const
{
	byteStream->WriteBytes((char*)&this->type, 1);

	switch (this->type)
	{
		case Type::INTERNAL:
		{
			if (!this->node[0]->Serialize(byteStream))
				return false;

			if (!this->node[1]->Serialize(byteStream))
				return false;

			break;
		}
		case Type::LEAF:
		{
			if (byteStream->WriteBytes(&this->character, 1) != 1)
				return false;

			break;
		}
	}

	return true;
}

/*static*/ HuffmanCode::Node* HuffmanCode::Node::Deserialize(ByteStream* byteStream)
{
	UniquePtr<Node> node(new Node());

	if (byteStream->ReadBytes((char*)&node->type, 1) != 1)
		return nullptr;

	switch (node->type)
	{
		case Type::INTERNAL:
		{
			node->node[0] = Node::Deserialize(byteStream);
			if (!node->node[0])
				return nullptr;

			node->node[1] = Node::Deserialize(byteStream);
			if (!node->node[1])
				return nullptr;

			break;
		}
		case Type::LEAF:
		{
			if (byteStream->ReadBytes(&node->character, 1) != 1)
				return nullptr;

			break;
		}
	}

	return node.Release();
}

void HuffmanCode::Node::PopulateCodeMap(HashMap<char, DArray<char>>& codeMap, DArray<char>& treePath) const
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