#include "UltraUtilities/Containers/BTree.h"

using namespace UU;

//--------------------------------------------- BTree ---------------------------------------------

BTree::BTree(int minDegree /*= 2*/)
{
	this->numKeys = 0;
	this->minDegree = minDegree;
	this->rootNode = nullptr;
}

/*virtual*/ BTree::~BTree()
{
}

int BTree::GetNumKeys() const
{
	return this->numKeys;
}

int BTree::GetMinDegree() const
{
	return this->minDegree;
}

int BTree::GetMaxDegree() const
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

		int i = 0;
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

	BTreeNode* node = this->rootNode;

	while (true)
	{
		int i = -1;
		if (node->FindKeyIndex(givenKey, i))
		{
			if (node->IsLeaf())
			{
				if (removedKey)
					*removedKey = node->keyArray[i];
				else
					delete node->keyArray[i];
				
				node->keyArray.ShiftRemove(i);
				this->numKeys--;
				break;
			}

			BTreeNode* nodeA = node->childNodeArray[i];
			BTreeNode* nodeB = node->childNodeArray[i + 1];

			if (nodeA->keyArray.GetSize() == this->minDegree - 1 && nodeB->keyArray.GetSize() == this->minDegree - 1)
			{
				// Push the key down the tree and merge adjacent siblings.

				BTreeKey* key = node->keyArray[i];

				node->keyArray.ShiftRemove(i);
				node->childNodeArray.ShiftRemove(i + 1);

				nodeA->keyArray.Push(key);

				for (int j = 0; j < (int)nodeB->keyArray.GetSize(); j++)
					nodeA->keyArray.Push(nodeB->keyArray[j]);

				for (int j = 0; j < (int)nodeB->childNodeArray.GetSize(); j++)
					nodeA->childNodeArray.Push(nodeB->childNodeArray[j]);

				nodeB->keyArray.SetSize(0);
				nodeB->childNodeArray.SetSize(0);
				delete nodeB;

				node = nodeA;
			}
			else
			{
				// At this point, I'm not sure if it matters if we go delete
				// the predecessor or the successor.  Either one would be fine,
				// I would think.

				if (nodeA->keyArray.GetSize() > this->minDegree - 1)
				{
					BTreeNode* leafNode = nodeA;
					while (!leafNode->IsLeaf())
						leafNode = leafNode->childNodeArray[leafNode->childNodeArray.GetSize() - 1];

					BTreeKey* predecessorKey = leafNode->keyArray[leafNode->keyArray.GetSize() - 1];
					if (!this->RemoveKey(predecessorKey))
						return false;

					node->keyArray[i] = predecessorKey;
					break;
				}
				else if (nodeB->keyArray.GetSize() > this->minDegree - 1)
				{
					BTreeNode* leafNode = nodeB;
					while (!leafNode->IsLeaf())
						leafNode = leafNode->childNodeArray[0];

					BTreeKey* successorKey = leafNode->keyArray[0];
					if (!this->RemoveKey(successorKey))
						return false;

					node->keyArray[i] = successorKey;
					break;
				}
				else
				{
					// The tree is not a BTree.
					UU_ASSERT(false);
					return false;
				}
			}
		}
		else
		{
			// The caller tried to remove a key that is not present in the tree.
			if (node->IsLeaf())
				return false;

			bool found = node->FindChildOrKeyInsertionIndex(givenKey, i);
			UU_ASSERT(found);

			BTreeNode* childNode = node->childNodeArray[i];
			if (childNode->keyArray.GetSize() < this->minDegree)
			{
				if (i - 1 >= 0 && node->childNodeArray[i - 1]->keyArray.GetSize() >= this->minDegree)
				{
					BTreeNode* siblingNode = node->childNodeArray[i - 1];

					BTreeKey* keyA = node->keyArray[i - 1];
					BTreeKey* keyB = siblingNode->keyArray[siblingNode->keyArray.GetSize() - 1];

					siblingNode->keyArray.Pop();
					node->keyArray.ShiftInsert(i - 1, keyB);
					node->keyArray.ShiftRemove(i);
					childNode->keyArray.ShiftInsert(0, keyA);

					if (!childNode->IsLeaf())
					{
						BTreeNode* grandChild = siblingNode->childNodeArray[siblingNode->childNodeArray.GetSize() - 1];
						siblingNode->childNodeArray.Pop();
						childNode->childNodeArray.ShiftInsert(0, grandChild);
						grandChild->parentNode = childNode;
					}
				}
				else if (i + 1 < (int)node->childNodeArray.GetSize() && node->childNodeArray[i + 1]->keyArray.GetSize() >= this->minDegree)
				{
					BTreeNode* siblingNode = node->childNodeArray[i + 1];

					BTreeKey* keyA = node->keyArray[i];
					BTreeKey* keyB = siblingNode->keyArray[0];

					siblingNode->keyArray.ShiftRemove(0);
					node->keyArray.ShiftInsert(i, keyB);
					node->keyArray.ShiftRemove(i + 1);
					childNode->keyArray.Push(keyA);

					if (!childNode->IsLeaf())
					{
						BTreeNode* grandChild = siblingNode->childNodeArray[0];
						siblingNode->childNodeArray.ShiftRemove(0);
						childNode->childNodeArray.Push(grandChild);
						grandChild->parentNode = childNode;
					}
				}
				else if (i - 1 >= 0)
				{
					BTreeKey* key = node->keyArray[i - 1];
					node->keyArray.ShiftRemove(i - 1);

					BTreeNode* siblingNode = node->childNodeArray[i - 1];
					node->childNodeArray.ShiftRemove(i);

					siblingNode->keyArray.Push(key);
					for (int j = 0; j < (int)childNode->keyArray.GetSize(); j++)
						siblingNode->keyArray.Push(childNode->keyArray[j]);

					for (int j = 0; j < (int)childNode->childNodeArray.GetSize(); j++)
						siblingNode->childNodeArray.Push(childNode->childNodeArray[j]);

					childNode->keyArray.SetSize(0);
					childNode->childNodeArray.SetSize(0);
					delete childNode;

					childNode = siblingNode;
				}
				else if (i + 1 < (int)node->childNodeArray.GetSize())
				{
					BTreeKey* key = node->keyArray[i];
					node->keyArray.ShiftRemove(i);

					BTreeNode* siblingNode = node->childNodeArray[i + 1];
					node->childNodeArray.ShiftRemove(i + 1);

					childNode->keyArray.Push(key);
					for (int j = 0; j < (int)siblingNode->keyArray.GetSize(); j++)
						childNode->keyArray.Push(siblingNode->keyArray[j]);

					for (int j = 0; j < (int)siblingNode->childNodeArray.GetSize(); j++)
						childNode->childNodeArray.Push(siblingNode->childNodeArray[j]);

					siblingNode->keyArray.SetSize(0);
					siblingNode->childNodeArray.SetSize(0);
					delete siblingNode;
				}
			}

			node = childNode;
		}
	}

	if (this->rootNode->keyArray.GetSize() == 0)
	{
		if (this->rootNode->childNodeArray.GetSize() != 1)
			this->rootNode = nullptr;
		else
		{
			node = this->rootNode;
			this->rootNode = node->childNodeArray[0];
			node->childNodeArray.SetSize(0);
			delete node;
			this->rootNode->parentNode = nullptr;
		}
	}

	return true;
}

bool BTree::AllLeafNodesAtSameDepth() const
{
	if (!this->rootNode)
		return true;

	int maxDepth = 0;
	int currentDepth = 1;
	return this->rootNode->AllLeafNodesAtSameDepth(maxDepth, currentDepth);
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

int BTreeNode::GetNumKeys() const
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
	int i = 0;

	if (this->FindKeyIndex(givenKey, i))
	{
		if (node)
			*node = this;

		return this->keyArray[i];
	}

	if (this->IsLeaf())
		return nullptr;

	if (this->FindChildOrKeyInsertionIndex(givenKey, i))
		return this->childNodeArray[i]->FindKey(givenKey, node);

	return nullptr;
}

bool BTreeNode::FindKeyIndex(BTreeKey* givenKey, int& i)
{
	for (i = 0; i < this->keyArray.GetSize(); i++)
		if (this->keyArray[i]->IsEqualTo(givenKey))
			return true;

	return false;
}

bool BTreeNode::FindChildOrKeyInsertionIndex(BTreeKey* givenKey, int& i)
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

	for (int j = 0; j < this->tree->GetMinDegree() - 1; j++)
	{
		auto movedKey = this->keyArray[this->tree->GetMinDegree() + j];
		newNode->keyArray.Push(movedKey);
	}

	this->keyArray.SetSize(this->tree->GetMinDegree() - 1);

	if (!this->IsLeaf())
	{
		for (int j = 0; j < this->tree->GetMinDegree(); j++)
		{
			auto movedChild = this->childNodeArray[this->tree->GetMinDegree() + j];
			newNode->childNodeArray.Push(movedChild);
			movedChild->parentNode = newNode;
		}

		this->childNodeArray.SetSize(this->tree->GetMinDegree());
	}

	if (this->parentNode)
	{
		int i = this->parentNode->childNodeArray.Find(this);
		UU_ASSERT(i != -1);
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
	for (int i = 0; i < sourceNode->keyArray.GetSize(); i++)
		destinationNode->keyArray.Push(sourceNode->keyArray[i]);

	for (int i = 0; i < sourceNode->childNodeArray.GetSize(); i++)
		destinationNode->childNodeArray.Push(sourceNode->childNodeArray[i]);

	sourceNode->keyArray.SetSize(0);
	sourceNode->childNodeArray.SetSize(0);
	delete sourceNode;
}

bool BTreeNode::AllLeafNodesAtSameDepth(int& maxDepth, int currentDepth) const
{
	if (this->IsLeaf())
	{
		if (maxDepth == 0)
			maxDepth = currentDepth;

		if (maxDepth != currentDepth)
			return false;

		return true;
	}

	for (int i = 0; i < this->childNodeArray.GetSize(); i++)
	{
		const BTreeNode* childNode = this->childNodeArray[i];
		if (!childNode->AllLeafNodesAtSameDepth(maxDepth, currentDepth + 1))
			return false;
	}

	return true;
}

bool BTreeNode::DegreesValid() const
{
	if (!this->IsRoot() && !this->IsLeaf())
		if (!(this->tree->GetMinDegree() <= this->childNodeArray.GetSize() && this->childNodeArray.GetSize() <= this->tree->GetMaxDegree()))
			return false;

	for (int i = 0; i < this->childNodeArray.GetSize(); i++)
	{
		const BTreeNode* childNode = this->childNodeArray[i];
		if (!childNode->DegreesValid())
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