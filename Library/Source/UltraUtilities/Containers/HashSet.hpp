#pragma once

#include "UltraUtilities/Containers/HashMap.hpp"

namespace UU
{
	/**
	 * This is a templatized wrapper for the @ref HashTable class.
	 * It is provided as a more convenient way to use the table.
	 * Note that unlike @ref RBSet, this set does not impose or
	 * define any ordering of its elements.  Also, hash tables
	 * have better time-complexity, on average, for insertion and
	 * removal than self-balancing binary trees, provided the
	 * hash table is big enough and the hash function is good.
	 */
	template<typename K>
	class UU_API HashSet
	{
	public:
		HashSet()
		{
		}

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
			HashMapKey<K> tableKey(key);
			auto node = static_cast<HashMapNode<int>*>(this->table.FindNode(&tableKey));
			return node != nullptr;
		}

		/**
		 * Insert the given key into this set.  Failure occurs
		 * if the given key already exists in the set.
		 */
		bool Insert(K key)
		{
			auto node = new HashMapNode();
			node->SetKey(new HashMapKey<K>(key));
			if (!this->table.InsertNode(node))
			{
				delete node;
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
			HashMapKey<K> treeKey(key);
			HashMapNode* node = this->table.FindNode(&treeKey);
			if (!node)
				return false;
			this->table.RemoveNode(node);
			delete node;
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
		 * Provide access to the hash table being wrapped by this template class.
		 */
		HashTable& GetHashTree() { return this->table; }

		// TODO: Provide support for ranged-for loop syntax.

	private:
		HashTable table;
	};
}