#pragma once

#include "UltraUtilities/Containers/RBMap.hpp"

namespace UU
{
	/**
	 * This is a templatized wrapper for the @ref RBTree class.
	 * It is provided as a more convenient way to use the tree.
	 */
	template<typename K>
	class UU_API RBSet
	{
	public:
		/**
		 * This provides bracket-syntax for membership queries.
		 */
		bool operator[](K key)
		{
			return this->Find(key);
		}

		/**
		 * Indicate whether the given key is a member of this set.
		 */
		bool Find(K key)
		{
			RBMapKey<K> treeKey(key);
			auto node = static_cast<RBMapNode<int>*>(this->tree.FindNode(&treeKey));
			return node != nullptr;
		}

		/**
		 * Insert the given key into this set.  Failure occurs
		 * if the given key already exists in the set.
		 */
		bool Insert(K key)
		{
			auto treeNode = new RBTreeNode();
			treeNode->SetKey(new RBMapKey<K>(key));
			if (!this->tree.InsertNode(treeNode))
			{
				delete treeNode;
				return false;
			}
			return true;
		}

		/**
		 * Remove the given key from this set.  Failure can
		 * occur if the given key does not already exist in this set.
		 */
		bool Remove(K key)
		{
			RBMapKey<K> treeKey(key);
			RBTreeNode* treeNode = this->tree.FindNode(&treeKey);
			if (!treeNode)
				return false;
			this->tree.RemoveNode(treeNode);
			delete treeNode;
			return true;
		}

		/**
		 * Remove all keys.
		 */
		void Clear()
		{
			this->tree.Clear();
		}

		/**
		 * Indicate how many keys are stored in the set.
		 */
		unsigned int GetNumKeys() const { return this->tree.GetNumNodes(); }

		/**
		 * Provide access to the tree being wrapped by this template class.
		 */
		RBTree& GetTree() { return this->tree; }

	private:
		RBTree tree;
	};
}