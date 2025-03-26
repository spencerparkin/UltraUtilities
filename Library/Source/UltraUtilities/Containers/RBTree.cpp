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
	return nullptr;
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