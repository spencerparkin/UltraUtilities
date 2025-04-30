#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/LinkedList.h"

namespace UU
{
	class HashTableKey;
	class HashTableNode;

	/**
	 * 
	 */
	class UU_API HashTable
	{
	public:
		HashTable(unsigned int tableSize = 2048);
		virtual ~HashTable();

		/**
		 * Find the node in this hash table having the given key, if any, and
		 * return it.  If not found, null is returned.
		 */
		HashTableNode* FindNode(const HashTableKey* key);

		/**
		 * Insert the given node into this hash table.  Note that it must
		 * be allocated by the user and given a key before this call.
		 */
		bool InsertNode(HashTableNode* newNode);

		/**
		 * Find and remove the node in this hash table having the given key, if any,
		 * and return it.  If not found, null is returned.  The caller takes ownership
		 * of the memory for the returned node.
		 */
		HashTableNode* RemoveNode(const HashTableKey* key);

		/**
		 * Remove the given node from this hash table.  Failure can
		 * occur if the given node is not a member of this hash table.
		 * The caller takes ownership of the memory for the node if
		 * removal is successful.
		 */
		bool RemoveNode(HashTableNode* oldNode);

		/**
		 * This is like @ref RemoveNode, but here we delete the memory
		 * taken up by the node, if any, having the given key.
		 */
		bool DeleteNode(HashTableKey* key);

		/**
		 * Remove all nodes from this hash table.
		 */
		void Clear();

		unsigned int GetNumNodes() const { return this->numNodes; }

		unsigned int GetTableSize() const { return this->tableSize; }

		LinkedList* GetTable() { return this->table; }

	private:
		LinkedList* table;
		unsigned int tableSize;
		unsigned int numNodes;
	};

	/**
	 * 
	 */
	class UU_API HashTableKey
	{
	public:
		virtual unsigned int Hash(unsigned int tableSize) const = 0;
		virtual bool operator==(const HashTableKey& key) const = 0;
	};

	/**
	 * 
	 */
	class UU_API HashTableNode : public LinkedListNode
	{
		friend class HashTable;

	public:
		HashTableNode();
		virtual ~HashTableNode();

		void SetKey(HashTableKey* key);
		HashTableKey* GetKey();

	private:
		HashTable* table;
		HashTableKey* key;
	};
}