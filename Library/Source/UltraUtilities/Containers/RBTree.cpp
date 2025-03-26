#include "UltraUtilities/Containers/RBTree.h"

using namespace UU;

//-------------------------- RBTreeBase --------------------------

RBTreeBase::RBTreeBase()
{
}

/*virtual*/ RBTreeBase::~RBTreeBase()
{
}

//-------------------------- RBTreeNodeBase --------------------------

RBTreeNodeBase::RBTreeNodeBase()
{
	this->leftChildNode = nullptr;
	this->rightChildNode = nullptr;
	this->parentNode = nullptr;
}

/*virtual*/ RBTreeNodeBase::~RBTreeNodeBase()
{
}

/*virtual*/ long RBTreeNodeBase::Key() const
{
	return 0;
}

/*virtual*/ bool RBTreeNodeBase::operator<(const RBTreeNodeBase* node) const
{
	return this->Key() < node->Key();
}

/*virtual*/ bool RBTreeNodeBase::operator>(const RBTreeNodeBase* node) const
{
	return this->Key() > node->Key();
}

/*virtual*/ bool RBTreeNodeBase::operator==(const RBTreeNodeBase* node) const
{
	return this->Key() == node->Key();
}