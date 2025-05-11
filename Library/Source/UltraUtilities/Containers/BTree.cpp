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
}

/*virtual*/ BTreeNode::~BTreeNode()
{
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
	return this->childNodeList.GetNumNodes() == 0;
}

bool BTreeNode::IsFull() const
{
	if (!this->tree)
		return false;

	return this->keyList.GetNumNodes() == this->tree->GetMaxDegree() - 1;
}

BTreeKey* BTreeNode::FindKey(BTreeKeyValue keyValue)
{
	for (LinkedListNode* keyNode = this->keyList.GetHead(); keyNode != nullptr; keyNode = keyNode->GetNext())
	{
		auto key = static_cast<BTreeKey*>(keyNode);
		if (key->GetKey() == keyValue)
			return key;
	}

	if (this->IsLeaf())
		return nullptr;

	LinkedListNode* keyNode = nullptr;
	LinkedListNode* childNode = nullptr;

	keyNode = this->keyList.GetHead();
	childNode = this->childNodeList.GetHead();
	if (keyValue < static_cast<BTreeKey*>(keyNode)->GetKey())
		return static_cast<BTreeNode*>(childNode)->FindKey(keyValue);

	keyNode = this->keyList.GetTail();
	childNode = this->childNodeList.GetTail();
	if (keyValue > static_cast<BTreeKey*>(keyNode)->GetKey())
		return static_cast<BTreeNode*>(childNode)->FindKey(keyValue);

	LinkedListNode* keyNodeA = this->keyList.GetHead();
	LinkedListNode* keyNodeB = keyNodeA->GetNext();
	childNode = this->childNodeList.GetHead()->GetNext();
	while (keyNodeA && keyNodeB)
	{
		BTreeKeyValue keyValueA = static_cast<BTreeKey*>(keyNodeA)->GetKey();
		BTreeKeyValue keyValueB = static_cast<BTreeKey*>(keyNodeB)->GetKey();

		if (keyValueA < keyValue && keyValue < keyValueB)
			return static_cast<BTreeNode*>(childNode)->FindKey(keyValue);

		keyNodeA = keyNodeB;
		keyNodeB = keyNodeB->GetNext();
		childNode = childNode->GetNext();
	}

	return nullptr;
}

//--------------------------------------------- BTreeKey ---------------------------------------------

BTreeKey::BTreeKey()
{
}

/*virtual*/ BTreeKey::~BTreeKey()
{
}