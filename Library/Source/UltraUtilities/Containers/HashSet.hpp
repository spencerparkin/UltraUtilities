#pragma once

#include "UltraUtilities/Containers/HashMap.hpp"
#include "UltraUtilities/Memory/ObjectHeap.hpp"

namespace UU
{
	template<typename K> class HashSetIterator;

	/**
	 * This is a templatized wrapper for the @ref HashTable class.
	 * It is provided as a more convenient way to use the table.
	 * Note that unlike @ref RBSet, this set does not impose or
	 * define any ordering of its elements.  Also, hash tables
	 * have better time-complexity, on average, for insertion and
	 * removal than self-balancing binary trees, provided the
	 * hash table is big enough and the hash function is good.
	 */
	template<typename K, typename NH = DefaultObjectHeap<HashTableNode>, typename KH = DefaultObjectHeap<HashMapKey<K>>>
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
			HashMapKey<K> mapKey;
			mapKey.value = key;
			auto node = this->table.FindNode(&mapKey);
			return node != nullptr;
		}

		/**
		 * Insert the given key into this set.  Failure occurs
		 * if the given key already exists in the set.
		 */
		bool Insert(K key)
		{
			auto node = this->nodeHeap.Allocate();
			auto setKey = this->keyHeap.Allocate();
			setKey->value = key;
			node->SetKey(setKey);
			if (!this->table.InsertNode(node))
			{
				this->keyHeap.Deallocate(static_cast<HashMapKey<K>*>(node->GetKey()));
				this->nodeHeap.Deallocate(node);
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
			HashMapKey<K> setKey;
			setKey.value = key;
			HashTableNode* node = this->table.FindNode(&setKey);
			if (!node)
				return false;
			this->table.RemoveNode(node);
			this->keyHeap.Deallocate(static_cast<HashMapKey<K>*>(node->GetKey()));
			this->nodeHeap.Deallocate(node);
			return true;
		}

		/**
		 * Remove all keys.
		 */
		void Clear()
		{
			this->table.Clear();
		}

		/**
		 * Indicate how many keys are stored in the set.
		 */
		unsigned int GetNumKeys() const { return this->table.GetNumNodes(); }

		/**
		 * Provide access to the hash table being wrapped by this template class.
		 */
		HashTable& GetHashTree() { return this->table; }

		/**
		 * This is provided to support the ranged for-loop syntax.
		 */
		HashSetIterator<K> begin()
		{
			return HashSetIterator<K>(&this->table);
		}

		/**
		 * This is the end sentinal for the ranged for-loop support.
		 */
		int end()
		{
			return int(this->table.GetTableSize()) - 1;
		}

	private:
		HashTable table;
		NH nodeHeap;
		KH keyHeap;
	};

	/**
	 * This is used internally by the @ref HashSet class to
	 * support the ranged for-loop syntax of C++.
	 */
	template<typename K>
	class UU_API HashSetIterator
	{
	public:
		HashSetIterator(HashTable* hashTable)
		{
			this->hashTable = hashTable;
			this->node = nullptr;
			this->i = -1;
			this->Advance();
		}

		void operator++()
		{
			this->Advance();
		}

		bool operator==(int i)
		{
			return this->i == i;
		}

		K operator*()
		{
			return static_cast<HashMapKey<K>*>(this->node->GetKey())->value;
		}

	private:
		void Advance()
		{
			if (this->node)
				this->node = static_cast<HashTableNode*>(this->node->GetNext());
			while (!this->node && this->i < int(this->hashTable->GetTableSize()) - 1)
				this->node = static_cast<HashTableNode*>(hashTable->GetTable()[++this->i].GetHead());
		}

		int i;
		HashTableNode* node;
		HashTable* hashTable;
	};
}