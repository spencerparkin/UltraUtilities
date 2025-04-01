#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/List.hpp"

namespace UU
{
	class RBTreeNode;
	class RBTreeKey;

	/**
	 * This is an implementation of a red/black tree.  This is a
	 * type of self-balancing binary search tree.
	 * 
	 * See: Introduction to Algorithms by Cormen, et. al.
	 */
	class UU_API RBTree
	{
		friend class RBTreeNode;

	public:
		RBTree();
		virtual ~RBTree();

		/**
		 * Efficiently find a node of the tree using the given key.
		 * Do not delete the given node, if any.
		 */
		RBTreeNode* FindNode(const RBTreeKey* key);

		/**
		 * Efficiently insert the given node into this tree.
		 * It must be allocated and given a key by the caller.
		 * This tree takes ownership of the memory for the given
		 * node unless failure occurs.  Failure can occur here
		 * if a node already exists having the same key.
		 */
		bool InsertNode(RBTreeNode* newNode);

		/**
		 * Efficiently remove the node, if any, with the given key.
		 * Ownership of the memory for the returned node is given
		 * to the caller.  Null is returned if no node having the
		 * given key exists.
		 */
		RBTreeNode* RemoveNode(const RBTreeKey* key);

		/**
		 * Efficiently remove the key for the given node from this tree.
		 * Once removed, the caller is responsible for deleting
		 * the memory associated with the returned node.  Failure
		 * can occur here if the given node is not a member of this
		 * tree.  Note that the returned node is not necessarily
		 * going to be the same as the one given!
		 */
		bool RemoveNode(RBTreeNode*& oldNode);

		/**
		 * This is the same as @ref RemoveNode, except that we
		 * take care here of deleting the removed node.  False is
		 * returned here if no node having the given key exists.
		 */
		bool DeleteNode(const RBTreeKey* key);

		/**
		 * Indicate how many nodes/keys are present in this tree.
		 */
		unsigned int GetNumNodes() const { return this->numNodes; }

		/**
		 * Remove all key/value pairs (nodes) from this tree, making it empty.
		 */
		void Clear();

		/**
		 * Find and return the node in this tree having the smallest key.
		 */
		RBTreeNode* FindMinimum();

		/**
		 * Find and return the node in this tree having the largest key.
		 */
		RBTreeNode* FindMaximum();

		/**
		 * This is used purely for diagnostic purposes to verify
		 * that the tree is indeed a valid binary search tree.
		 */
		bool IsBinaryTree() const;

		/**
		 * This is used purely for diagnostic purposes to verify
		 * that this tree is indeed a valid red/black tree according
		 * to definition.
		 */
		bool IsRedBlackTree() const;

		/**
		 * Provide read-only access to the root node.
		 */
		const RBTreeNode* GetRootNode() const { return this->rootNode; }

	private:
		RBTreeNode* rootNode;
		unsigned int numNodes;
	};

	/**
	 * Instances of this class are nodes of a red/black tree.
	 */
	class UU_API RBTreeNode
	{
		friend class RBTree;

	public:
		RBTreeNode();
		virtual ~RBTreeNode();

		/**
		 * This should be overridden to copy the satilate data
		 * owned by the given node to this node.
		 */
		virtual void CopyValue(RBTreeNode* node);

		/**
		 * Assuming the tree is valid, this efficiently finds the
		 * node, if any, with the given key.
		 */
		RBTreeNode* FindNode(const RBTreeKey* key);

		/**
		 * Verify that the sub-tree rooted at this node is a valid
		 * binary tree according to definition.
		 */
		bool IsBinaryTree() const;

		/**
		 * This provides a convenient way to visit all nodes in
		 * the tree unconditionally.  You can early-out by returning
		 * false from the given lambda.  As the name suggests, we
		 * perform a depth-first traversal of the tree.
		 */
		template<typename Lambda>
		bool ForAllNodesDFS(Lambda callback) const
		{
			if (!callback(this))
				return false;
			if (this->leftChildNode && !this->leftChildNode->ForAllNodesDFS(callback))
				return false;
			if (this->rightChildNode && !this->rightChildNode->ForAllNodesDFS(callback))
				return false;
			return true;
		}

		/**
		 * This is just like @ref ForAllNodesDFS, except here we're
		 * doing a breadth-first traversal of the tree.
		 */
		template<typename Lambda>
		bool ForAllNodesBFS(Lambda callback) const
		{
			List<const RBTreeNode*> queue;
			queue.PushBack(this);
			while (queue.GetNumValues() > 0)
			{
				const RBTreeNode* node = nullptr;
				queue.PopBack(&node);
				if (!callback(node))
					return false;
				if (node->leftChildNode)
					queue.PushBack(node->leftChildNode);
				if (node->rightChildNode)
					queue.PushBack(node->rightChildNode);
			}
			return true;
		}

		/**
		 * This is used purely for diagnostic purposes to verify that the
		 * sub-tree rooted at this node is balanced according to one of the
		 * defining properties of a red/black tree.
		 */
		bool IsBalanced(int& blackHeight, int blackCount) const;

		/**
		 * Return true if and only if this node is a leaf node.
		 */
		bool IsLeaf() const;

		/**
		 * Return true if and only if this node is the root node.
		 */
		bool IsRoot() const;

		/**
		 * Return true if and only if this node has both a left and right node.
		 */
		bool IsInternal() const;

		/**
		 * Find the node in the tree having the smallest key greater than this node's key.
		 */
		RBTreeNode* FindSuccessor();

		/**
		 * Find the node in the tree having the largest key less than this node's key.
		 */
		RBTreeNode* FindPredecessor();

		enum Color
		{
			RED,
			BLACK
		};

		Color GetColor() const { return this->color; }
		const RBTreeNode* GetLeftNode() const { return this->leftChildNode; }
		const RBTreeNode* GetRightNode() const { return this->rightChildNode; }

		/**
		 * Assign a key to this node.  Note that we do not take ownership of the memory
		 * here.  The caller needs to make sure that the memory is cleaned up after this
		 * node goes out of scope.
		 */
		void SetKey(RBTreeKey* key);

		/**
		 * Get the key assigned to this node or null if none is assigned.
		 */
		RBTreeKey* GetKey();

	private:
		RBTreeNode** FindParentBranchPointer();

		enum RotationDirection
		{
			LEFT,
			RIGHT
		};

		void Rotate(RotationDirection rotationDirection);

		RBTree* tree;
		RBTreeKey* key;
		RBTreeNode* leftChildNode;
		RBTreeNode* rightChildNode;
		RBTreeNode* parentNode;
		Color color;
	};

	/**
	 * These are keys used in a red/black tree.
	 */
	class UU_API RBTreeKey
	{
	public:
		virtual bool operator<(const RBTreeKey& key) const = 0;
		virtual bool operator>(const RBTreeKey& key) const = 0;
		virtual bool operator==(const RBTreeKey& key) const = 0;
		virtual bool operator<=(const RBTreeKey& key) const;
		virtual bool operator>=(const RBTreeKey& key) const;
	};
}