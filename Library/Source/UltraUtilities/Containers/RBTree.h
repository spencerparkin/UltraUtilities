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
		bool InsertNode(RBTreeNode* node);

		/**
		 * Efficiently remove the node, if any, with the given key.
		 * Ownership of the memory for the returned node is given
		 * to the caller.  Null is returned if no node having the
		 * given key exists.
		 */
		RBTreeNode* RemoveNode(const RBTreeKey* key);

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

		RBTreeNode* FindNode(const RBTreeKey* key);

		bool IsBinaryTree() const;

		bool ForAllNodes(std::function<bool(const RBTreeNode* node)> callback) const;

		/**
		 * This is used purely for diagnostic purposes to verify that the
		 * sub-tree rooted at this node is balanced according to one of the
		 * defining properties of a red/black tree.
		 */
		bool IsBalanced(int& blackHeight, int blackCount) const;

		bool IsLeaf() const;
		bool IsRoot() const;

		enum Color
		{
			RED,
			BLACK
		};

		Color GetColor() const { return this->color; }
		const RBTreeNode* GetLeftNode() const { return this->leftChildNode; }
		const RBTreeNode* GetRightNode() const { return this->rightChildNode; }

	private:
		enum RotationDirection
		{
			LEFT,
			RIGHT
		};

		void Rotate(RotationDirection rotationDirection, RBTree* tree);

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
			auto treeNode = static_cast<RBTreeNode_<V>*>(this->tree.RemoveNode(&treeNode));
			if (!treeNode)
				return false;
			if (value)
				*value = treeNode->value;
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