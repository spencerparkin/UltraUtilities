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
	return false;
}

BTreeKey* BTree::FindKey(BTreeKeyValue keyValue)
{
	if (!this->rootNode)
		return nullptr;

	return this->rootNode->FindKey(keyValue);
}

bool BTree::RemoveKey(BTreeKeyValue keyValue)
{
	return false;
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

BTreeKey* BTreeNode::FindKey(BTreeKeyValue keyValue)
{
	for (unsigned int i = 0; i < this->keyArray.GetSize(); i++)
		if (this->keyArray[i]->GetKeyValue() == keyValue)
			return this->keyArray[i];

	if (this->IsLeaf())
		return nullptr;

	if (keyValue < this->keyArray[0]->GetKeyValue())
		return this->childNodeArray[0]->FindKey(keyValue);

	if (keyValue > this->keyArray[this->keyArray.GetSize() - 1]->GetKeyValue())
		return this->childNodeArray[this->childNodeArray.GetSize() - 1]->FindKey(keyValue);

	for (unsigned int i = 0; i + 1 < this->keyArray.GetSize(); i++)
	{
		BTreeKey* keyA = this->keyArray[i];
		BTreeKey* keyB = this->keyArray[i + 1];

		if (keyA->GetKeyValue() < keyValue && keyValue < keyB->GetKeyValue())
			return this->childNodeArray[i + 1]->FindKey(keyValue);
	}

	return nullptr;
}

bool BTreeNode::Split()
{
	if (!this->tree || !this->parentNode)
		return false;

	if (this->parentNode->IsFull())
		return false;

	if (!this->IsFull())
		return false;

	//...move a key into the parent, and move keys into a new sibling...
	return false;
}

//--------------------------------------------- BTreeKey ---------------------------------------------

BTreeKey::BTreeKey()
{
}

/*virtual*/ BTreeKey::~BTreeKey()
{
}