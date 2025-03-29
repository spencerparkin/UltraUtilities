#pragma once

#include "UltraUtilities/Containers/HashTable.h"

namespace UU
{
	template<typename K> class HashMapKey;
	template<typename V> class HashMapNode;

	/**
	 * This is a template wrapper around the @ref HashTable class.
	 */
	template<typename K, typename V>
	class HashMap
	{
	public:
		HashMap()
		{
		}

		virtual ~HashMap()
		{
		}

		/**
		 * This provides bracket-syntax access to the hash map.
		 * Note that if you ask for the value of a key that
		 * does not exist in the map, then you may get an
		 * uninitialized value if the value type does not
		 * have a constructor.
		 */
		V operator[](K key)
		{
			V value;
			this->Find(key, &value);
			return value;
		}

		/**
		 * Find a value by key.  If a value pointer is not given,
		 * then this method can be used to check for existence
		 * of a given key in the hash map.
		 */
		bool Find(K key, V* value = nullptr)
		{
			HashMapKey<K> tableKey(key);
			auto node = static_cast<HashMapNode<V>*>(this->table.FindNode(&tableKey));
			if (!node)
				return false;
			if (value)
				*value = node->value;
			return true;
		}

		/**
		 * Insert a value at the given key.  If a value already
		 * exists at the given key, it is replaced.
		 */
		bool Insert(K key, V value)
		{
			HashMapKey<K> tableKey(key);
			auto node = static_cast<HashMapNode<V>*>(this->table.FindNode(&tableKey));
			if (node)
				node->value = value;
			else
			{
				node = new HashMapNode<V>(value);
				node->SetKey(new HashMapKey<K>(key));
				if (!this->table.InsertNode(node))
				{
					delete node;
					return false;
				}
			}
			return true;
		}

		/**
		 * Remove the value at the given key, if any.
		 * The value at the given key is returned if desired.
		 */
		bool Remove(K key, V* value = nullptr)
		{
			HashTableKey<K> tableKey(key);
			HashTableNode* node = this->table.FindNode(&tableKey);
			if (!node)
				return false;
			if (value)
				*value = static_cast<HashMapNode<V>*>(node)->value;
			this->table.RemoveNode(node);
			delete node;
			return true;
		}

		/**
		 * Remove all key/value pairs.
		 */
		void Clear()
		{
			this->table.Clear();
		}

		/**
		 * Indicate how many key/value pairs are stored in the table.
		 */
		unsigned int GetNumPairs() const { return this->table.GetNumNodes(); }

		/**
		 * Provide access to the hash table being wrapped by this template class.
		 */
		HashTable& GetHashTable() { return this->table; }

		// TODO: Provide support for ranged-for loop syntax.

	private:
		HashTable table;
	};

	/**
	 * This is used internally by the @ref HashMap class.
	 */
	template<typename V>
	class HashMapNode : public HashTableNode
	{
	public:
		HashMapNode(V value)
		{
			this->value = value;
		}

	public:
		V value;
	};

	/**
	 * This is used internally by the @ref HashMap class.
	 */
	template<typename K>
	class HashMapKey : public HashTableKey
	{
	public:
		virtual unsigned int Hash(unsigned int tableSize) override
		{
			return K::Hash(this->value, tableSize);
		}

		virtual bool operator==(const HashTableKey& key) const override
		{
			return K::Equal(this->value, static_cast<const HashMapKey<V>*>(&key)->value);
		}

	public:
		K value;
	};

	/**
	 * Provide a specialization that uses the division method of hashing.
	 */
	template<>
	class HashMapKey<int> : public HashTableKey
	{
	public:
		virtual unsigned int Hash(unsigned int tableSize) const override
		{
			return this->value % tableSize;
		}

		virtual bool operator==(const HashTableKey& key) const override
		{
			return this->value == static_cast<const HashMapKey<int>*>(&key)->value;
		}

	public:
		int value;
	};
}