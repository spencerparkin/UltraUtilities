#include "UltraUtilities/DisjointSetForest.h"

using namespace UU;

DSFNode::DSFNode()
{
	this->parentNode = nullptr;
}

/*virtual*/ DSFNode::~DSFNode()
{
}

/*static*/ bool DSFNode::MembersOfSameSet(const DSFNode* nodeA, const DSFNode* nodeB)
{
	return nodeA->FindSetRepresentative() == nodeB->FindSetRepresentative();
}

const DSFNode* DSFNode::FindSetRepresentative() const
{
	if (!this->parentNode)
		return this;

	if (!this->parentNode->parentNode)
		return this->parentNode;

	const DSFNode* repNode = this->parentNode->FindSetRepresentative();

	// This line of code is purely an optimization, and is not needed for correctess.
	this->parentNode = const_cast<DSFNode*>(repNode);

	return repNode;
}

/*static*/ bool DSFNode::MergeSets(DSFNode* nodeA, DSFNode* nodeB)
{
	DSFNode* repNodeA = const_cast<DSFNode*>(nodeA->FindSetRepresentative());
	DSFNode* repNodeB = const_cast<DSFNode*>(nodeB->FindSetRepresentative());

	if (repNodeA == repNodeB)
		return false;

	// Arbitrarily parent one to the other.  It doesn't matter which we choose.
	repNodeA->parentNode = repNodeB;
	return true;
}