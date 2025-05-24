#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/DArray.hpp"

namespace UU
{
	class BTreeNode;
	class BTreeKey;

	/**
	 * This is an implementation of B-tree.  This is a
	 * type of self-balancing search tree with an arbitrary
	 * branching factor.
	 *
	 * See: Introduction to Algorithms by Cormen, et. al.
	 */
	class UU_API BTree
	{
		friend class BTreeNode;

	public:
		BTree(unsigned int minDegree = 2);
		virtual ~BTree();

		/**
		 * Return the number of keys currently present in this tree.
		 */
		unsigned int GetNumKeys() const;

		/**
		 * Return the minimum number of child nodes per internal node.
		 */
		unsigned int GetMinDegree() const;

		/**
		 * Return the maximum number of child nodes per internal node.
		 */
		unsigned int GetMaxDegree() const;

		/**
		 * Find the key in this tree that is equal to the given key.  Null is returned
		 * here if no such key can be found.
		 * 
		 * One might ask: Why look for a key when you already have it?  The idea is that
		 * the key stored in the tree has the desired satellite data, while the given
		 * key does not.
		 */
		BTreeKey* FindKey(BTreeKey* givenKey);

		/**
		 * Insert the given key into this tree.  The given key should be allocated
		 * by the caller.  The tree takes ownership of the memory.  Failure can
		 * occur here if the given key already exists in the tree.  If failure
		 * occurs, the tree does not take ownership of the memory.
		 */
		bool InsertKey(BTreeKey* key);

		/**
		 * Remove the key from this tree that is equal to the given key.  Failure occurs
		 * here if no such key exists within the tree.
		 */
		bool RemoveKey(BTreeKey* givenKey);

		/**
		 * Used only for diagnostic purposes, here we verify that all leaf nodes of
		 * the tree are at the same depth.
		 */
		bool IsBalanced() const;

		/**
		 * Used only for diagnostic purposes, here we verify that all nodes (except
		 * for the root) have a branch-factor within the minimum and maximum degree.
		 */
		bool DegreesValid() const;

	private:
		unsigned int numKeys;
		unsigned int minDegree;			///< This is the minimum number of children per internal node of the tree.  The maximum is always twice this.
		BTreeNode* rootNode;
	};

	/**
	 * These serve as the nodes of a B-tree.  User's of the @ref BTree class
	 * should never have to use this class directly.
	 */
	class UU_API BTreeNode
	{
		friend class BTree;

	public:
		BTreeNode();
		virtual ~BTreeNode();

		BTree* GetTree();
		void SetTree(BTree* tree);

		bool IsLeaf() const;
		bool IsFull() const;

		BTreeKey* FindKey(BTreeKey* givenKey);
		bool FindKeyIndex(BTreeKey* givenKey, unsigned int& i);
		bool FindChildOrKeyInsertionIndex(BTreeKey* givenKey, unsigned int& i);

		bool Split();

		bool IsBalanced(unsigned int& maxDepth, unsigned int currentDepth) const;
		bool DegreesValid() const;

	private:
		BTree* tree;
		DArray<BTreeNode*> childNodeArray;
		DArray<BTreeKey*> keyArray;
		BTreeNode* parentNode;
	};

	/**
	 * These are the keys inserted into and removed from a B-tree.
	 * A derivative of this class should be made so as to provide
	 * satellite data in the tree.
	 */
	class UU_API BTreeKey
	{
	public:
		BTreeKey();
		virtual ~BTreeKey();

		virtual bool IsEqualTo(const BTreeKey* key) const = 0;
		virtual bool IsLessThan(const BTreeKey* key) const = 0;
		virtual bool IsGreaterThan(const BTreeKey* key) const = 0;
	};
}