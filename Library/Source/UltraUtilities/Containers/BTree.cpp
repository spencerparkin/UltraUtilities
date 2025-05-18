#include "UltraUtilities/Containers/BTree.h"

using namespace UU;

//--------------------------------------------- BTree ---------------------------------------------

BTree::BTree(unsigned int minDegree /*= 2*/)
{
	this->minDegree = minDegree;
	this->rootNode = nullptr;
}

/*virtual*/ BTree::~BTree()
{
}

unsigned int BTree::GetMinDegree() const
{
	return this->minDegree;
}

unsigned int BTree::GetMaxDegree() const
{
	return 2 * this->minDegree;
}

bool BTree::InsertKey(BTreeKey* key)
{
	if (!this->rootNode)
	{
		this->rootNode = new BTreeNode();
		this->rootNode->tree = this;
		this->rootNode->keyArray.Push(key);
		return true;
	}

	auto node = this->rootNode;

	while (true)
	{
		if (node->IsFull())
		{
			bool nodeSplit = node->Split();
			UU_ASSERT(nodeSplit);
			node = node->parentNode;
			UU_ASSERT(node != nullptr);
		}

		unsigned int i = 0;
		if (node->FindKeyIndex(key, i))
			return false;
		
		bool indexFound = node->FindChildOrKeyInsertionIndex(key, i);
		UU_ASSERT(indexFound);

		if (node->IsLeaf())
		{
			node->keyArray.ShiftInsert(i, key);
			break;
		}
		
		node = node->childNodeArray[i];
		UU_ASSERT(node != nullptr);
	}

	return true;
}

BTreeKey* BTree::FindKey(BTreeKey* givenKey)
{
	if (!this->rootNode)
		return nullptr;

	return this->rootNode->FindKey(givenKey);
}

bool BTree::RemoveKey(BTreeKey* givenKey)
{
	// TODO: Write this.
	return false;
}

bool BTree::IsBalanced() const
{
	if (!this->rootNode)
		return true;

	unsigned int maxDepth = 0;
	unsigned int currentDepth = 1;
	return this->rootNode->IsBalanced(maxDepth, currentDepth);
}

//--------------------------------------------- BTreeNode ---------------------------------------------

BTreeNode::BTreeNode()
{
	this->tree = nullptr;
	this->parentNode = nullptr;
}

/*virtual*/ BTreeNode::~BTreeNode()
{
	for (BTreeNode* childNode : this->childNodeArray)
		delete childNode;

	for (BTreeKey* key : this->keyArray)
		delete key;
}

BTree* BTreeNode::GetTree()
{
	return this->tree;
}

void BTreeNode::SetTree(BTree* tree)
{
	this->tree = tree;
}

bool BTreeNode::IsLeaf() const
{
	return this->childNodeArray.GetSize() == 0;
}

bool BTreeNode::IsFull() const
{
	if (!this->tree)
		return false;

	return this->keyArray.GetSize() == this->tree->GetMaxDegree() - 1;
}

BTreeKey* BTreeNode::FindKey(BTreeKey* givenKey)
{
	unsigned int i = 0;

	if (this->FindKeyIndex(givenKey, i))
		return this->keyArray[i];

	if (this->IsLeaf())
		return nullptr;

	if (this->FindChildOrKeyInsertionIndex(givenKey, i))
		return this->childNodeArray[i]->FindKey(givenKey);

	return nullptr;
}

bool BTreeNode::FindKeyIndex(BTreeKey* givenKey, unsigned int& i)
{
	for (i = 0; i < this->keyArray.GetSize(); i++)
		if (this->keyArray[i]->IsEqualTo(givenKey))
			return true;

	return false;
}

bool BTreeNode::FindChildOrKeyInsertionIndex(BTreeKey* givenKey, unsigned int& i)
{
	if (givenKey->IsLessThan(this->keyArray[0]))
		i = 0;
	else if (givenKey->IsGreaterThan(this->keyArray[this->keyArray.GetSize() - 1]))
		i = this->keyArray.GetSize();
	else
	{
		for (i = 0; i + 1 < this->keyArray.GetSize(); i++)
		{
			BTreeKey* keyA = this->keyArray[i];
			BTreeKey* keyB = this->keyArray[i + 1];

			if (givenKey->IsGreaterThan(keyA) && givenKey->IsLessThan(keyB))
			{
				i++;
				break;
			}
		}
	}

	return true;
}

bool BTreeNode::Split()
{
	if (!this->tree)
		return false;

	if (!this->IsFull())
		return false;

	if (this->parentNode && this->parentNode->IsFull())
		return false;

	auto liftedKey = this->keyArray[this->tree->GetMinDegree() - 1];

	auto newNode = new BTreeNode();
	newNode->tree = this->tree;
	newNode->parentNode = this->parentNode;

	for (unsigned int j = 0; j < this->tree->GetMinDegree() - 1; j++)
	{
		auto movedKey = this->keyArray[this->tree->GetMinDegree() + j];
		newNode->keyArray.Push(movedKey);
	}

	this->keyArray.SetSize(this->tree->GetMinDegree() - 1);

	if (!this->IsLeaf())
	{
		for (unsigned int j = 0; j < this->tree->GetMinDegree(); j++)
		{
			auto movedChild = this->childNodeArray[this->tree->GetMinDegree() + j];
			newNode->childNodeArray.Push(movedChild);
		}

		this->childNodeArray.SetSize(this->tree->GetMinDegree());
	}

	if (this->parentNode)
	{
		unsigned int i = this->parentNode->childNodeArray.Find(this);
		this->parentNode->childNodeArray.ShiftInsert(i + 1, newNode);
		this->parentNode->keyArray.ShiftInsert(i, liftedKey);
	}
	else
	{
		auto newRoot = new BTreeNode();
		this->tree->rootNode = newRoot;
		newRoot->tree = this->tree;
		newRoot->childNodeArray.Push(this);
		newRoot->childNodeArray.Push(newNode);
		newRoot->keyArray.Push(liftedKey);
		this->parentNode = newRoot;
		newNode->parentNode = newRoot;
	}

	return true;
}

bool BTreeNode::IsBalanced(unsigned int& maxDepth, unsigned int currentDepth) const
{
	if (this->IsLeaf())
	{
		if (maxDepth == 0)
			maxDepth = currentDepth;

		if (maxDepth != currentDepth)
			return false;

		return true;
	}

	for (unsigned int i = 0; i < this->childNodeArray.GetSize(); i++)
	{
		const BTreeNode* childNode = this->childNodeArray[i];
		if (!childNode->IsBalanced(maxDepth, currentDepth + 1))
			return false;
	}

	return true;
}

//--------------------------------------------- BTreeKey ---------------------------------------------

BTreeKey::BTreeKey()
{
}

/*virtual*/ BTreeKey::~BTreeKey()
{
}