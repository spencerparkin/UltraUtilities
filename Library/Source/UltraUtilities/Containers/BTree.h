#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/LinkedList.h"

namespace UU
{
	class BTreeNode;
	class BTreeKey;

	typedef long long BTreeKeyValue;

	/**
	 * This is an implementation of B-tree.  This is a
	 * type of self-balancing search tree with an arbitrary
	 * branching factor.
	 *
	 * See: Introduction to Algorithms by Cormen, et. al.
	 */
	class UU_API BTree
	{
	public:
		BTree(unsigned int minDegree = 2);
		virtual ~BTree();

		/**
		 * Return the minimum number of child nodes per internal node.
		 */
		unsigned int GetMinDegree() const;

		/**
		 * Return the maximum number of child nodes per internal node.
		 */
		unsigned int GetMaxDegree() const;

		/**
		 * Find the key in this tree having the given value.  Null is returned
		 * here if no such key can be found.
		 */
		BTreeKey* FindKey(BTreeKeyValue keyValue);

		/**
		 * Insert the given key into this tree.  The given key should be allocated
		 * by the caller.  The tree takes ownership of the memory.  Failure can
		 * occur here if the given key already exists in the tree.  If failure
		 * occurs, the tree does not take ownership of the memory.
		 */
		bool InsertKey(BTreeKey* key);

		/**
		 * Remove the key from this tree having the given value.  Failure occurs
		 * here if no such key exists within the tree.
		 */
		bool RemoveKey(BTreeKeyValue keyValue);

	private:
		unsigned int minDegree;			///< This is the minimum number of children per internal node of the tree.  The maximum is always twice this.
		BTreeNode* rootNode;
	};

	/**
	 * These serve as the nodes of a B-tree.  User's of the @ref BTree class
	 * should never have to use this class directly.
	 */
	class UU_API BTreeNode : public LinkedListNode
	{
	public:
		BTreeNode();
		virtual ~BTreeNode();

		BTree* GetTree();
		void SetTree(BTree* tree);

		bool IsLeaf() const;
		bool IsFull() const;

		BTreeKey* FindKey(BTreeKeyValue keyValue);

	private:
		BTree* tree;
		LinkedList childNodeList;
		LinkedList keyList;
	};

	/**
	 * These are the keys inserted into and removed from a B-tree.
	 * A derivative of this class should be made so as to provide
	 * satellite data in the tree.
	 */
	class UU_API BTreeKey : public LinkedListNode
	{
	public:
		BTreeKey();
		virtual ~BTreeKey();

		/**
		 * Override this method to provide a key for the tree.
		 * Note that for the tree to function correctly, the result
		 * returned here should never very from call to call.
		 */
		virtual BTreeKeyValue GetKey() = 0;
	};

	/**
	 * This template is provided for convenience in creating B-tree keys
	 * based on a custom user type.  This type could not only provide the
	 * key's integer value, but could also represent the satellite data.
	 */
	template<typename T>
	class UU_API BTreeTypedKey : public BTreeKey
	{
	public:
		BTreeTypedKey(T value)
		{
			this->value = value;
		}

		virtual ~BTreeTypedKey()
		{
		}

		/**
		 * Note that the type for the template could implement a cast operator which
		 * would get called here to provide the key.
		 */
		virtual BTreeKeyValue GetKey() override
		{
			return BTreeKeyValue(this->value);
		}

	protected:
		T value;
	};
}