#include "UltraUtilities/Containers/RBTree.h"

using namespace UU;

//------------------------------ RBTree ------------------------------

RBTree::RBTree()
{
	this->rootNode = nullptr;
}

/*virtual*/ RBTree::~RBTree()
{
	delete this->rootNode;
}

RBTreeNode* RBTree::FindNode(const RBTreeKey* key)
{
	if (!this->rootNode)
		return nullptr;

	return this->rootNode->FindNode(key);
}

bool RBTree::InsertNode(RBTreeNode* node)
{
	return false;
}

RBTreeNode* RBTree::RemoveNode(const RBTreeKey* key)
{
	return nullptr;
}

bool RBTree::DeleteNode(const RBTreeKey* key)
{
	return false;
}

bool RBTree::IsBinaryTree() const
{
	if (!this->rootNode)
		return true;

	return this->rootNode->IsBinaryTree();
}

bool RBTree::IsRedBlackTree() const
{
	if (!this->IsBinaryTree())
		return false;

	if (!this->rootNode)
		return true;

	if (!this->rootNode->ForAllNodes([](const RBTreeNode* node) -> bool
		{
			if (node->GetColor() == RBTreeNode::RED)
			{
				if (node->GetLeftNode() && node->GetLeftNode()->GetColor() != RBTreeNode::BLACK)
					return false;

				if (node->GetRightNode() && node->GetRightNode()->GetColor() != RBTreeNode::BLACK)
					return false;
			}

			return true;
		}))
	{
		return false;
	}

	if (!this->rootNode->ForAllNodes([](const RBTreeNode* node) -> bool
		{
			int blackHeight = -1;
			return node->IsBalanced(blackHeight, 0);
		}))
	{
		return false;
	}

	return true;
}

bool RBTreeNode::IsBalanced(int& blackHeight, int blackCount) const
{
	if (this->IsLeaf())
	{
		if (blackHeight == -1)
		{
			blackHeight = blackCount;
			return true;
		}

		return blackHeight == blackCount;
	}

	if (this->color == Color::BLACK)
		blackCount++;

	if (this->leftChildNode && !this->leftChildNode->IsBalanced(blackHeight, blackCount))
		return false;

	if (this->rightChildNode && !this->rightChildNode->IsBalanced(blackHeight, blackCount))
		return false;

	return true;
}

bool RBTreeNode::IsLeaf() const
{
	return !this->leftChildNode && !this->rightChildNode;
}

bool RBTreeNode::IsRoot() const
{
	return !this->parentNode;
}

//------------------------------ RBTreeKey ------------------------------

/*virtual*/ bool RBTreeKey::operator<=(const RBTreeKey& key) const
{
	return *this == key || *this < key;
}

/*virtual*/ bool RBTreeKey::operator>=(const RBTreeKey& key) const
{
	return *this == key || *this > key;
}

//------------------------------ RBTreeNode ------------------------------

RBTreeNode::RBTreeNode()
{
	this->key = nullptr;
	this->leftChildNode = nullptr;
	this->rightChildNode = nullptr;
	this->parentNode = nullptr;
	this->color = Color::BLACK;
}

/*virtual*/ RBTreeNode::~RBTreeNode()
{
	delete this->key;
	delete this->leftChildNode;
	delete this->rightChildNode;
}

RBTreeNode* RBTreeNode::FindNode(const RBTreeKey* key)
{
	if (*this->key == *key)
		return this;

	if (this->leftChildNode && *this->key < *key)
		return this->leftChildNode->FindNode(key);

	if (this->rightChildNode && *this->key > *key)
		return this->rightChildNode->FindNode(key);

	return nullptr;
}

void RBTreeNode::Rotate(RotationDirection rotationDirection, RBTree* tree)
{
	RBTreeNode** branch = nullptr;

	if (!this->parentNode)
		branch = &tree->rootNode;
	else if (this->parentNode->rightChildNode == this)
		branch = &this->parentNode->rightChildNode;
	else if (this->parentNode->leftChildNode == this)
		branch = &this->parentNode->leftChildNode;
	else
	{
		assert(false);
		return;
	}

	switch (rotationDirection)
	{
		case LEFT:
		{
			if (this->rightChildNode)
			{
				RBTreeNode* node = this->rightChildNode;

				if (this->rightChildNode->leftChildNode)
				{
					this->rightChildNode = this->rightChildNode->leftChildNode;
					this->rightChildNode->parentNode = this;
				}

				node->parentNode = this->parentNode;
				*branch = node;

				node->leftChildNode = this;
				this->parentNode = node;
			}
			break;
		}
		case RIGHT:
		{
			if (this->leftChildNode)
			{
				RBTreeNode* node = this->leftChildNode;

				if (this->leftChildNode->rightChildNode)
				{
					this->leftChildNode = this->leftChildNode->rightChildNode;
					this->leftChildNode->parentNode = this;
				}

				node->parentNode = this->parentNode;
				*branch = node;

				node->rightChildNode = this;
				this->parentNode = node;
			}

			break;
		}
	}	
}

bool RBTreeNode::IsBinaryTree() const
{
	if (this->leftChildNode)
	{
		if (!this->leftChildNode->IsBinaryTree())
			return false;

		const RBTreeKey* largestKey = nullptr;
		this->leftChildNode->ForAllNodes([this, &largestKey](const RBTreeNode* node) -> bool
			{
				if (!largestKey || *largestKey < *node->key)
					largestKey = node->key;
				return true;
			});

		if (*largestKey >= *this->key)
			return false;
	}

	if (this->rightChildNode)
	{
		if (!this->rightChildNode->IsBinaryTree())
			return false;

		const RBTreeKey* smallestKey = nullptr;
		this->rightChildNode->ForAllNodes([this, &smallestKey](const RBTreeNode* node) -> bool
			{
				if (!smallestKey || *smallestKey > *node->key)
					smallestKey = node->key;
				return true;
			});

		if (*smallestKey <= *this->key)
			return false;
	}

	return true;
}

bool RBTreeNode::ForAllNodes(std::function<bool(const RBTreeNode* node)> callback) const
{
	if (!callback(this))
		return false;

	if (this->leftChildNode && !this->leftChildNode->ForAllNodes(callback))
		return false;

	if (this->rightChildNode && !this->rightChildNode->ForAllNodes(callback))
		return false;

	return true;
}