#include "UltraUtilities/Containers/RBTree.h"

using namespace UU;

//------------------------------ RBTree ------------------------------

RBTree::RBTree()
{
	this->rootNode = nullptr;
	this->numNodes = 0;
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

void RBTree::Clear()
{
	delete this->rootNode;
	this->rootNode = nullptr;
	this->numNodes = 0;
}

bool RBTree::InsertNode(RBTreeNode* newNode)
{
	if (!newNode || !newNode->key || newNode->tree)
		return false;

	if (!this->rootNode)
		this->rootNode = newNode;
	else
	{
		RBTreeNode* node = this->rootNode;
		while (true)
		{
			if (*newNode->key < *node->key)
			{
				if (!node->leftChildNode)
				{
					node->leftChildNode = newNode;
					newNode->parentNode = node;
					break;
				}

				node = node->leftChildNode;
			}
			else if (*newNode->key > *node->key)
			{
				if (!node->rightChildNode)
				{
					node->rightChildNode = newNode;
					newNode->parentNode = node;
					break;
				}

				node = node->rightChildNode;
			}
			else
			{
				assert(*newNode->key == *node->key);
				return false;
			}
		}
	}

	newNode->tree = this;
	newNode->color = RBTreeNode::RED;
	this->numNodes++;

	// Restore the red/black properties of the tree.  (i.e., rebalance the tree.)
	RBTreeNode* node = newNode;
	while (node && node->parentNode && node->parentNode->color == RBTreeNode::RED)
	{
		RBTreeNode* grandParentNode = node->parentNode->parentNode;
		assert(grandParentNode != nullptr);		// Our coloring of the root black should guarentee this condition.

		RBTreeNode::RotationDirection grandParentRotationDirection = RBTreeNode::RotationDirection::LEFT;
		RBTreeNode* uncleNode = nullptr;
		
		if (node->parentNode == grandParentNode->leftChildNode)
		{
			grandParentRotationDirection = RBTreeNode::RotationDirection::RIGHT;
			uncleNode = grandParentNode->rightChildNode;
		}
		else if (node->parentNode == grandParentNode->rightChildNode)
		{
			grandParentRotationDirection = RBTreeNode::RotationDirection::LEFT;
			uncleNode = grandParentNode->leftChildNode;
		}

		if (!uncleNode || uncleNode->color == RBTreeNode::BLACK)
		{
			// Note that if we get here, once these rotations are complete, we should exit the loop immediately.

			if (node->parentNode->rightChildNode == node && grandParentRotationDirection == RBTreeNode::RotationDirection::RIGHT)
				node->parentNode->Rotate(RBTreeNode::RotationDirection::LEFT);
			else if (node->parentNode->leftChildNode == node && grandParentRotationDirection == RBTreeNode::RotationDirection::LEFT)
				node->parentNode->Rotate(RBTreeNode::RotationDirection::RIGHT);

			grandParentNode->Rotate(grandParentRotationDirection);
			grandParentNode->color = RBTreeNode::RED;
			node->parentNode->color = RBTreeNode::BLACK;
			node = node->parentNode;
		}
		else
		{
			node->parentNode->color = RBTreeNode::BLACK;
			uncleNode->color = RBTreeNode::BLACK;
			grandParentNode->color = RBTreeNode::RED;
			node = grandParentNode;
		}
	}

	this->rootNode->color = RBTreeNode::BLACK;

	return true;
}

RBTreeNode* RBTree::RemoveNode(const RBTreeKey* key)
{
	RBTreeNode* node = this->FindNode(key);
	if (this->RemoveNode(node))
		return node;

	return nullptr;
}

bool RBTree::RemoveNode(RBTreeNode*& oldNode)
{
	if (!oldNode || oldNode->tree != this)
		return false;

	if (oldNode->IsInternal())
	{
		RBTreeNode* node = oldNode->FindSuccessor();	// Could use predecessor here too; doesn't matter which.
		assert(!node->IsInternal());

		oldNode->CopyValue(node);

		if (!this->RemoveNode(node))
		{
			assert(false);
			return false;
		}

		oldNode = node;
		return true;
	}
	
	RBTreeNode** branch = oldNode->FindParentBranchPointer();
	assert(branch);

	if (oldNode->leftChildNode)
	{
		oldNode->leftChildNode->parentNode = oldNode->parentNode;
		*branch = oldNode->leftChildNode;
	}
	else if (oldNode->rightChildNode)
	{
		oldNode->rightChildNode->parentNode = oldNode->parentNode;
		*branch = oldNode->rightChildNode;
	}
	else
	{
		*branch = nullptr;
	}

	oldNode->parentNode = nullptr;
	oldNode->tree = nullptr;
	oldNode->leftChildNode = nullptr;
	oldNode->rightChildNode = nullptr;
	this->numNodes--;

	// Restore the red/black properties of the tree.  (i.e., rebalance the tree.)
	RBTreeNode* extraBlackNode = *branch;
	if (extraBlackNode && extraBlackNode->parentNode)
	{
		while (extraBlackNode != this->rootNode && extraBlackNode->color == RBTreeNode::BLACK)
		{
			if (extraBlackNode->parentNode->leftChildNode == extraBlackNode)
			{
				RBTreeNode* siblingNode = extraBlackNode->parentNode->rightChildNode;

				if (siblingNode->color == RBTreeNode::RED)
				{
					// Case 1
					extraBlackNode->parentNode->color = RBTreeNode::RED;
					siblingNode->color = RBTreeNode::BLACK;
					extraBlackNode->parentNode->Rotate(RBTreeNode::RotationDirection::LEFT);
				}
				else if (siblingNode->color == RBTreeNode::BLACK)
				{
					if (!siblingNode->rightChildNode || siblingNode->rightChildNode->color == RBTreeNode::BLACK)
					{
						if (!siblingNode->leftChildNode || siblingNode->leftChildNode->color == RBTreeNode::BLACK)
						{
							// Case 2
							siblingNode->color = RBTreeNode::RED;
							extraBlackNode = extraBlackNode->parentNode;
						}
						else if (siblingNode->leftChildNode->color == RBTreeNode::RED)
						{
							// Case 3
							siblingNode->color = RBTreeNode::RED;
							siblingNode->leftChildNode->color = RBTreeNode::BLACK;
							siblingNode->Rotate(RBTreeNode::RotationDirection::RIGHT);
						}
					}
					else if (siblingNode->rightChildNode->color == RBTreeNode::RED)
					{
						// Case 4
						siblingNode->color = extraBlackNode->parentNode->color;
						siblingNode->rightChildNode->color = RBTreeNode::BLACK;
						extraBlackNode->parentNode->Rotate(RBTreeNode::RotationDirection::LEFT);
					}
				}
			}
			else if (extraBlackNode->parentNode->rightChildNode == extraBlackNode)
			{
				RBTreeNode* siblingNode = extraBlackNode->parentNode->leftChildNode;

				if (siblingNode->color == RBTreeNode::RED)
				{
					// Case 1
					extraBlackNode->parentNode->color = RBTreeNode::RED;
					siblingNode->color = RBTreeNode::BLACK;
					extraBlackNode->parentNode->Rotate(RBTreeNode::RotationDirection::RIGHT);
				}
				else if (siblingNode->color == RBTreeNode::BLACK)
				{
					if (!siblingNode->leftChildNode || siblingNode->leftChildNode->color == RBTreeNode::BLACK)
					{
						if (!siblingNode->rightChildNode || siblingNode->rightChildNode->color == RBTreeNode::BLACK)
						{
							// Case 2
							siblingNode->color = RBTreeNode::RED;
							extraBlackNode = extraBlackNode->parentNode;
						}
						else if (siblingNode->rightChildNode->color == RBTreeNode::RED)
						{
							// Case 3
							siblingNode->color = RBTreeNode::RED;
							siblingNode->rightChildNode->color = RBTreeNode::BLACK;
							siblingNode->Rotate(RBTreeNode::RotationDirection::LEFT);
						}
					}
					else if (siblingNode->leftChildNode->color == RBTreeNode::RED)
					{
						// Case 4
						siblingNode->color = extraBlackNode->parentNode->color;
						siblingNode->leftChildNode->color = RBTreeNode::BLACK;
						extraBlackNode->parentNode->Rotate(RBTreeNode::RotationDirection::RIGHT);
					}
				}
			}
		}

		this->rootNode->color = RBTreeNode::BLACK;
	}

	return true;
}

bool RBTree::DeleteNode(const RBTreeKey* key)
{
	RBTreeNode* node = this->RemoveNode(key);
	if (!node)
		return false;

	delete node;
	return true;
}

RBTreeNode* RBTree::FindMinimum()
{
	if (!this->rootNode)
		return nullptr;

	RBTreeNode* node = this->rootNode;
	while (true)
	{
		if (!node->leftChildNode)
			return node;

		node = node->leftChildNode;
	}

	return node;
}

RBTreeNode* RBTree::FindMaximum()
{
	if (!this->rootNode)
		return nullptr;

	RBTreeNode* node = this->rootNode;
	while (true)
	{
		if (!node->rightChildNode)
			return node;

		node = node->rightChildNode;
	}

	return node;
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
	if (this->color == Color::BLACK)
		blackCount++;

	if (this->IsLeaf())
	{
		if (blackHeight == -1)
		{
			blackHeight = blackCount;
			return true;
		}

		return blackHeight == blackCount;
	}

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

bool RBTreeNode::IsInternal() const
{
	return this->leftChildNode && this->rightChildNode;
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
	this->tree = nullptr;
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

void RBTreeNode::SetKey(RBTreeKey* key)
{
	delete this->key;
	this->key = key;
}

const RBTreeKey* RBTreeNode::GetKey() const
{
	return this->key;
}

RBTreeNode* RBTreeNode::FindSuccessor()
{
	if (this->rightChildNode)
	{
		RBTreeNode* node = this->rightChildNode;
		while (node->leftChildNode)
			node = node->leftChildNode;
		return node;
	}
	else
	{
		RBTreeNode* node = this;
		while (true)
		{
			RBTreeNode* parentNode = node->parentNode;
			if (!parentNode)
				break;

			if (parentNode->leftChildNode == node)
				return parentNode;

			node = parentNode;
		}
	}

	return nullptr;
}

RBTreeNode* RBTreeNode::FindPredecessor()
{
	if (this->leftChildNode)
	{
		RBTreeNode* node = this->leftChildNode;
		while (node->rightChildNode)
			node = node->rightChildNode;
		return node;
	}
	else
	{
		RBTreeNode* node = this;
		while (true)
		{
			RBTreeNode* parentNode = node->parentNode;
			if (!parentNode)
				break;

			if (parentNode->rightChildNode == node)
				return parentNode;

			node = parentNode;
		}
	}

	return nullptr;
}

/*virtual*/ void RBTreeNode::CopyValue(RBTreeNode* node)
{
	// We do nothing here by default.
}

RBTreeNode* RBTreeNode::FindNode(const RBTreeKey* key)
{
	if (*this->key == *key)
		return this;

	if (this->leftChildNode && *key < *this->key)
		return this->leftChildNode->FindNode(key);

	if (this->rightChildNode && *key > *this->key)
		return this->rightChildNode->FindNode(key);

	return nullptr;
}

RBTreeNode** RBTreeNode::FindParentBranchPointer()
{
	RBTreeNode** branch = nullptr;

	if (!this->parentNode)
		branch = &this->tree->rootNode;
	else if (this->parentNode->rightChildNode == this)
		branch = &this->parentNode->rightChildNode;
	else if (this->parentNode->leftChildNode == this)
		branch = &this->parentNode->leftChildNode;
	
	return branch;
}

void RBTreeNode::Rotate(RotationDirection rotationDirection)
{
	RBTreeNode** branch = this->FindParentBranchPointer();
	assert(branch != nullptr);

	switch (rotationDirection)
	{
		case LEFT:
		{
			if (!this->rightChildNode)
			{
				assert(false);
				return;
			}

			RBTreeNode* node = this->rightChildNode;

			if (!this->rightChildNode->leftChildNode)
				this->rightChildNode = nullptr;
			else
			{
				this->rightChildNode = this->rightChildNode->leftChildNode;
				this->rightChildNode->parentNode = this;
			}

			node->parentNode = this->parentNode;
			*branch = node;

			node->leftChildNode = this;
			this->parentNode = node;
			break;
		}
		case RIGHT:
		{
			if (!this->leftChildNode)
			{
				assert(false);
				return;
			}

			RBTreeNode* node = this->leftChildNode;

			if (!this->leftChildNode->rightChildNode)
				this->leftChildNode = nullptr;
			else
			{
				this->leftChildNode = this->leftChildNode->rightChildNode;
				this->leftChildNode->parentNode = this;
			}

			node->parentNode = this->parentNode;
			*branch = node;

			node->rightChildNode = this;
			this->parentNode = node;
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