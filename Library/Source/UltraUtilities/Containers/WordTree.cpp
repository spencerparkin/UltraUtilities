#include "UltraUtilities/Containers/WordTree.h"

using namespace UU;

//----------------------------------- WordTree -----------------------------------

WordTree::WordTree(Mode mode)
{
	this->mode = mode;
	this->rootNode = nullptr;
}

/*virtual*/ WordTree::~WordTree()
{
	delete this->rootNode;
}

bool WordTree::MakeWordTraversalParams(const char* word, WordTraversalParams& params) const
{
	int length = 0;
	while (word[length] != '\0')
		length++;

	if (length == 0)
		return false;

	params.delta = 0;
	params.start = 0;
	params.stop = 0;

	switch (mode)
	{
		case Mode::PrefixTree:
		{
			params.start = 0;
			params.stop = length;
			params.delta = 1;
			break;
		}
		case Mode::SuffixTree:
		{
			params.start = length - 1;
			params.stop = -1;
			params.delta = -1;
			break;
		}
		default:
		{
			return false;
		}
	}

	return true;
}

bool WordTree::AddWord(const char* word)
{
	if (!this->rootNode)
		this->rootNode = new Node();

	WordTraversalParams params{};
	if (!this->MakeWordTraversalParams(word, params))
		return false;

	bool wordAdded = false;
	Node* node = this->rootNode;
	for (int i = params.start; i != params.stop; i += params.delta)
	{
		unsigned char j = (unsigned char)word[i];
		
		if (!node->childNodeArray[j])
		{
			node->childNodeArray[j] = new Node();
			wordAdded = true;
		}

		node = node->childNodeArray[j];
	}

	return wordAdded;
}

bool WordTree::ContainsWord(const char* word, const Node** lastNode /*= nullptr*/) const
{
	if (!this->rootNode)
		return false;

	WordTraversalParams params{};
	if (!this->MakeWordTraversalParams(word, params))
		return false;

	const Node* node = this->rootNode;
	for (int i = params.start; i != params.stop; i += params.delta)
	{
		unsigned char j = (unsigned char)word[i];

		if (!node->childNodeArray[j])
			return false;

		node = node->childNodeArray[j];
	}

	if (lastNode)
		*lastNode = node;

	return true;
}

void WordTree::GetAllWordCompletions(const char* word, DArray<String>& completedWordArray) const
{
	completedWordArray.SetSize(0);

	const Node* node = nullptr;
	if (!this->ContainsWord(word, &node))
		return;

	if (!node)
		completedWordArray.Push(String(word));
	else
	{
		List<char> charList;
		for (int i = 0; word[i] != '\0'; i++)
			charList.PushBack(word[i]);

		node->ExploreAll(charList, completedWordArray, this->mode);
	}
}

//----------------------------------- WordTree::Node -----------------------------------
		
WordTree::Node::Node()
{
	for (int i = 0; i < 256; i++)
		this->childNodeArray[i] = nullptr;
}

/*virtual*/ WordTree::Node::~Node()
{
	for (int i = 0; i < 256; i++)
		delete this->childNodeArray[i];
}

void WordTree::Node::ExploreAll(List<char>& charList, DArray<String>& completedWordArray, Mode mode) const
{
	int callCount = 0;
	for (int i = 0; i < 256; i++)
	{
		const Node* childNode = this->childNodeArray[i];
		if (childNode)
		{
			switch (mode)
			{
			case Mode::PrefixTree:
				charList.PushBack((char)i);
				break;
			case Mode::SuffixTree:
				charList.PushFront((char)i);
				break;
			}

			callCount++;
			childNode->ExploreAll(charList, completedWordArray, mode);

			switch (mode)
			{
			case Mode::PrefixTree:
				charList.PopBack();
				break;
			case Mode::SuffixTree:
				charList.PopBack();
				break;
			}
		}
	}

	if (callCount == 0)
	{
		String completedWord;
		for (char ch : charList)
			completedWord.PushChar(ch);

		completedWordArray.Push(completedWord);
	}
}