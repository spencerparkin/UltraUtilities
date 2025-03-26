#pragma once

#include "UltraUtilities/Utilities.h"

namespace UU
{
	class RBTreeNode;
	class RBTreeKey;

	/**
	 * This is an implementation of a red/black tree.  This is a
	 * self-balancing binary search tree.
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
		 * Efficiently remove the given node from this tree.
		 * Once removed, the caller is responsible for deleting
		 * the memory associated with the returned node.  Failure
		 * can occur here if the given node is not a member of this
		 * tree.  Note that the returned node is not necessarily
		 * going to be the same as the given node!
		 */
		bool RemoveNode(RBTreeNode*& oldNode);

		/**
		 * This is the same as @ref RemoveNode, except that we
		 * take care here of deleting the removed node.  False is
		 * returned here if no node having the given key exists.
		 */
		bool DeleteNode(const RBTreeKey* key);

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

	private:
		RBTreeNode* rootNode;
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
		 * false from the given lambda.
		 */
		bool ForAllNodes(std::function<bool(const RBTreeNode* node)> callback) const;

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

	template<typename K> class RBTreeKey_;
	template<typename V> class RBTreeNode_;

	/**
	 * This is a templatized wrapper for the @ref RBTree class.
	 * It is provided as a more convenient way to use the tree.
	 */
	template<typename K, typename V>
	class UU_API RBTree_
	{
	public:
		/**
		 * Find a value by key.  If a value pointer is not given,
		 * then this method can be used to check for existance
		 * of the given key in the tree.
		 */
		bool Find(K key, V* value = nullptr)
		{
			RBTreeKey_<K> treeKey(key);
			auto node = static_cast<RBTreeNode_<V>*>(this->tree.FindNode(&treeKey));
			if (!node)
				return false;
			if (value)
				*value = node->value;
			return true;
		}

		/**
		 * Insert a value at the given key.  Failure occurs here
		 * if a value already exists in the tree at the given key.
		 */
		bool Insert(K key, V value)
		{
			auto treeNode = new RBTreeNode_<V>(value);
			treeNode->key = new RBTreeKey_<V>(key);
			if (!this->tree.InsertNode(treeNode))
			{
				delete treeNode;
				return false;
			}
			return true;
		}

		/**
		 * Remove the value at the given key, if any.
		 * The value at the given key is returned if desired.
		 */
		bool Remove(K key, V* value = nullptr)
		{
			RBTreeKey_<K> treeKey(key);
			RBTreeNode* treeNode = this->tree.FindNode(&treeKey);
			if (!treeNode)
				return false;
			if (value)
				*value = static_cast<RBTreeNode_<V>*>(treeNode)->value;
			this->tree.RemoveNode(treeNode);
			delete treeNode;
			return true;
		}

	private:
		RBTree tree;
	};

	/**
	 * This is used internally by the @ref RBTree_ class.
	 */
	template<typename V>
	class UU_API RBTreeNode_ : public RBTreeNode
	{
	public:
		RBTreeNode_(V value)
		{
			this->value = value;
		}

		virtual ~RBTreeNode_()
		{
		}

		virtual void CopyValue(RBTreeNode* node) override
		{
			this->value = static_cast<RBTreeNode_<V>*>(node)->value;
		}

	public:
		V value;
	};

	/**
	 * This is used internally by the @ref RBTree_ class.
	 */
	template<typename K>
	class UU_API RBTreeKey_ : public RBTreeKey
	{
	public:
		RBTreeKey_(K value)
		{
			this->value = value;
		}

		virtual bool operator<(const RBTreeKey& key) const override
		{
			return this->value < static_cast<const RBTreeKey_<K>*>(&key)->value;
		}

		virtual bool operator>(const RBTreeKey& key) const override
		{
			return this->value > static_cast<const RBTreeKey_<K>*>(&key)->value;
		}

		virtual bool operator==(const RBTreeKey& key) const override
		{
			return this->value == static_cast<const RBTreeKey_<K>*>(&key)->value;
		}

	public:
		K value;
	};
}