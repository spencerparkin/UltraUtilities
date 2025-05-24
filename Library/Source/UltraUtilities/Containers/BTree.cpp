#include "UltraUtilities/Containers/BTree.h"

using namespace UU;

//--------------------------------------------- BTree ---------------------------------------------

BTree::BTree(unsigned int minDegree /*= 2*/)
{
	this->numKeys = 0;
	this->minDegree = minDegree;
	this->rootNode = nullptr;
}

/*virtual*/ BTree::~BTree()
{
}

unsigned int BTree::GetNumKeys() const
{
	return this->numKeys;
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
		this->numKeys++;
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

	this->numKeys++;
	return true;
}

BTreeKey* BTree::FindKey(BTreeKey* givenKey)
{
	if (!this->rootNode)
		return nullptr;

	return this->rootNode->FindKey(givenKey);
}

bool BTree::RemoveKey(BTreeKey* givenKey, BTreeKey** removedKey /*= nullptr*/)
{
	if (!this->rootNode)
		return false;

	BTreeNode* node = nullptr;
	if (!this->rootNode->FindKey(givenKey, &node))
		return false;

	return this->RemoveKeyInNode(givenKey, removedKey, node);
}

bool BTree::RemoveKeyInNode(BTreeKey* givenKey, BTreeKey** removedKey, BTreeNode* node)
{
	BTreeKey* freeThisKey = nullptr;
	if (!removedKey)
		removedKey = &freeThisKey;

	unsigned int i = -1;
	bool found = node->FindKeyIndex(givenKey, i);
	UU_ASSERT(found);

	if (node->IsInternal())
	{
		// Note that a predecessor or successor in this case will always be a leaf node.
		// Arbitrarily choose the predecessor.  Both should exist, because a key with no
		// predecessor is in a leaf, and the same is true with the successor.
		BTreeNode* leafNode = node->childNodeArray[i];
		while (!leafNode->IsLeaf())
			leafNode = leafNode->childNodeArray[leafNode->childNodeArray.GetSize() - 1];
		BTreeKey* lastKey = leafNode->keyArray[leafNode->keyArray.GetSize() - 1];
		BTreeKey* lastKeyRemoved = nullptr;
		bool removed = this->RemoveKeyInNode(lastKey, &lastKeyRemoved, leafNode);
		UU_ASSERT(removed);
		*removedKey = node->keyArray[i];
		node->keyArray[i] = lastKey;
	}
	else
	{
		UU_ASSERT(node->IsLeaf());

		bool removalOkay = node->IsRoot() || node->GetNumKeys() > this->minDegree - 1;

		*removedKey = node->keyArray[i];
		node->keyArray.ShiftRemove(i);

		if (!removalOkay)
		{
			BTreeNode* parentNode = node->parentNode;
			UU_ASSERT(parentNode != nullptr);

			unsigned int j = -1;
			bool childFound = parentNode->FindChildOrKeyInsertionIndex(givenKey, j);
			UU_ASSERT(childFound);

			BTreeNode* leftSibling = (j - 1 >= 0) ? parentNode->childNodeArray[j - 1] : nullptr;
			BTreeNode* rightSibling = (j + 1 < parentNode->childNodeArray.GetSize()) ? parentNode->childNodeArray[j + 1] : nullptr;
			UU_ASSERT(!leftSibling || leftSibling->IsLeaf());
			UU_ASSERT(!rightSibling || rightSibling->IsLeaf());

			if (leftSibling)
			{
				node->keyArray.ShiftInsert(0, parentNode->keyArray[j]);

				if (leftSibling->GetNumKeys() > this->minDegree - 1)
				{
					parentNode->keyArray[j] = leftSibling->keyArray[leftSibling->keyArray.GetSize() - 1];
					leftSibling->keyArray.Pop();
				}
				else
				{
					BTreeNode::Merge(leftSibling, node);
					parentNode->keyArray.ShiftRemove(j);
					parentNode->childNodeArray.ShiftRemove(j);
				}
			}
			else if (rightSibling)
			{
				node->keyArray.Push(parentNode->keyArray[j]);

				if (rightSibling->GetNumKeys() > this->minDegree - 1)
				{
					parentNode->keyArray[j] = rightSibling->keyArray[0];
					rightSibling->keyArray.ShiftRemove(0);
				}
				else
				{
					BTreeNode::Merge(node, rightSibling);
					parentNode->keyArray.ShiftRemove(j + 1);
					parentNode->childNodeArray.ShiftRemove(j + 1);
				}
			}
			else
			{
				UU_ASSERT(false);
				return false;
			}
		}

		if (this->rootNode->childNodeArray.GetSize() == 1)
		{
			UU_ASSERT(this->rootNode->keyArray.GetSize() == 0);
			node = this->rootNode;
			this->rootNode = this->rootNode->childNodeArray[0];
			node->childNodeArray.SetSize(0);
			delete node;
		}
		else if (this->rootNode->childNodeArray.GetSize() == 0 && this->rootNode->keyArray.GetSize() == 0)
		{
			delete this->rootNode;
			this->rootNode = nullptr;
		}

		this->numKeys--;
	}

	delete freeThisKey;
	return true;
}

bool BTree::IsBalanced() const
{
	if (!this->rootNode)
		return true;

	unsigned int maxDepth = 0;
	unsigned int currentDepth = 1;
	return this->rootNode->IsBalanced(maxDepth, currentDepth);
}

bool BTree::DegreesValid() const
{
	if (!this->rootNode)
		return true;

	return this->rootNode->DegreesValid();
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

unsigned int BTreeNode::GetNumKeys() const
{
	return this->keyArray.GetSize();
}

bool BTreeNode::IsLeaf() const
{
	return this->childNodeArray.GetSize() == 0;
}

bool BTreeNode::IsInternal() const
{
	return !this->IsLeaf();
}

bool BTreeNode::IsRoot() const
{
	if (this->parentNode == nullptr)
	{
		UU_ASSERT(this == this->tree->rootNode);
		return true;
	}

	return false;
}

bool BTreeNode::IsFull() const
{
	if (!this->tree)
		return false;

	return this->keyArray.GetSize() == this->tree->GetMaxDegree() - 1;
}

BTreeKey* BTreeNode::FindKey(BTreeKey* givenKey, BTreeNode** node /*= nullptr*/)
{
	unsigned int i = 0;

	if (this->FindKeyIndex(givenKey, i))
	{
		if (node)
			*node = this;

		return this->keyArray[i];
	}

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

/*static*/ void BTreeNode::Merge(BTreeNode* destinationNode, BTreeNode* sourceNode)
{
	for (unsigned int i = 0; i < sourceNode->keyArray.GetSize(); i++)
		destinationNode->keyArray.Push(sourceNode->keyArray[i]);

	for (unsigned int i = 0; i < sourceNode->childNodeArray.GetSize(); i++)
		destinationNode->childNodeArray.Push(sourceNode->childNodeArray[i]);

	sourceNode->keyArray.SetSize(0);
	sourceNode->childNodeArray.SetSize(0);
	delete sourceNode;
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

bool BTreeNode::DegreesValid() const
{
	if (this == this->tree->rootNode)
		return true;

	if (this->tree->GetMinDegree() <= this->childNodeArray.GetSize() && this->childNodeArray.GetSize() <= this->tree->GetMaxDegree())
		return true;

	for (unsigned int i = 0; i < this->childNodeArray.GetSize(); i++)
	{
		const BTreeNode* childNode = this->childNodeArray[i];
		if (!childNode->DegreesValid())
			return false;
	}

	return false;
}

//--------------------------------------------- BTreeKey ---------------------------------------------

BTreeKey::BTreeKey()
{
}

/*virtual*/ BTreeKey::~BTreeKey()
{
}