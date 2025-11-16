#pragma once

#include "UltraUtilities/Containers/HashTable.h"
#include "UltraUtilities/Memory/ObjectHeap.hpp"

namespace UU
{
	template<typename K> class HashMapKey;
	template<typename V> class HashMapNode;
	template<typename K, typename V> class HashMapIterator;

	/**
	 * This is a template wrapper around the @ref HashTable class.
	 */
	template<typename K, typename V, typename NH = DefaultObjectHeap<HashMapNode<V>>, typename KH = DefaultObjectHeap<HashMapKey<K>>>
	class UU_API HashMap
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
			HashMapKey<K> mapKey;
			mapKey.value = key;
			auto node = static_cast<HashMapNode<V>*>(this->table.FindNode(&mapKey));
			if (!node)
				return false;
			if (value)
				*value = node->value;
			return true;
		}

		/**
		 * Find a pointer to a value by key.
		 */
		bool FindPtr(K key, V*& value)
		{
			HashMapKey<K> mapKey;
			mapKey.value = key;
			auto node = static_cast<HashMapNode<V>*>(this->table.FindNode(&mapKey));
			if (!node)
				return false;
			value = &node->value;
			return true;
		}

		/**
		 * Insert a value at the given key.  If a value already
		 * exists at the given key, it is replaced.
		 */
		bool Insert(K key, V value)
		{
			HashMapKey<K> mapKey;
			mapKey.value = key;
			auto node = static_cast<HashMapNode<V>*>(this->table.FindNode(&mapKey));
			if (node)
				node->value = value;
			else
			{
				node = this->nodeHeap.Allocate();
				node->value = value;
				node->SetKey(this->keyHeap.Allocate());
				static_cast<HashMapKey<K>*>(node->GetKey())->value = key;
				if (!this->table.InsertNode(node))
				{
					this->keyHeap.Deallocate(static_cast<HashMapKey<K>*>(node->GetKey()));
					this->nodeHeap.Deallocate(static_cast<HashMapNode<V>*>(node));
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
			HashMapKey<K> mapKey;
			mapKey.value = key;
			HashTableNode* node = this->table.FindNode(&mapKey);
			if (!node)
				return false;
			if (value)
				*value = static_cast<HashMapNode<V>*>(node)->value;
			this->table.RemoveNode(node);
			this->keyHeap.Deallocate(static_cast<HashMapKey<K>*>(node->GetKey()));
			this->nodeHeap.Deallocate(static_cast<HashMapNode<V>*>(node));
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

		/**
		 * This is provided to support the ranged for-loop syntax.
		 */
		HashMapIterator<K, V> begin()
		{
			return HashMapIterator<K, V>(&this->table);
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
	 * This is used internally by the @ref HashMap class to
	 * support ranged for-loop syntax in C++.
	 */
	template<typename K, typename V>
	class UU_API HashMapIterator
	{
	public:
		struct Pair
		{
			K key;
			V value;
		};

		HashMapIterator(HashTable* hashTable)
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

		Pair operator*()
		{
			Pair pair;
			pair.key = static_cast<HashMapKey<K>*>(this->node->GetKey())->value;
			pair.value = static_cast<HashMapNode<V>*>(this->node)->value;
			return pair;
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

	/**
	 * This is used internally by the @ref HashMap class.
	 */
	template<typename V>
	class UU_API HashMapNode : public HashTableNode
	{
	public:
		V value;
	};

	/**
	 * This is used internally by the @ref HashMap class.
	 */
	template<typename K>
	class UU_API HashMapKey : public HashTableKey
	{
	public:
		virtual unsigned int Hash(unsigned int tableSize) override
		{
			return K::Hash(this->value, tableSize);
		}

		virtual bool operator==(const HashTableKey& key) const override
		{
			return K::Equal(this->value, static_cast<const HashMapKey<K>*>(&key)->value);
		}

	public:
		K value;
	};

	/**
	 * Provide a specialization that uses integers and the division method of hashing.
	 */
	template<>
	class UU_API HashMapKey<int> : public HashTableKey
	{
	public:
		virtual unsigned int Hash(unsigned int tableSize) const override
		{
			return static_cast<unsigned int>(this->value) % tableSize;
		}

		virtual bool operator==(const HashTableKey& key) const override
		{
			return this->value == static_cast<const HashMapKey<int>*>(&key)->value;
		}

	public:
		int value;
	};

	/**
	 * Provide a specialization that uses long integers and the division method of hashing.
	 */
	template<>
	class UU_API HashMapKey<unsigned long long> : public HashTableKey
	{
	public:
		virtual unsigned int Hash(unsigned int tableSize) const override
		{
			return static_cast<unsigned int>(this->value % static_cast<unsigned long long>(tableSize));
		}

		virtual bool operator==(const HashTableKey& key) const override
		{
			return this->value == static_cast<const HashMapKey<int>*>(&key)->value;
		}

	public:
		unsigned long long value;
	};

	/**
	 * Provide a specialization that uses characters and the division method of hashing.
	 */
	template<>
	class UU_API HashMapKey<char> : public HashTableKey
	{
	public:
	public:
		virtual unsigned int Hash(unsigned int tableSize) const override
		{
			return static_cast<unsigned int>(this->value) % tableSize;
		}

		virtual bool operator==(const HashTableKey& key) const override
		{
			return this->value == static_cast<const HashMapKey<char>*>(&key)->value;
		}

	public:
		char value;
	};
}