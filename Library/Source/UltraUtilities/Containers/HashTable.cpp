#include "UltraUtilities/Containers/HashTable.h"

using namespace UU;

//------------------------------ HashTable ------------------------------

HashTable::HashTable(unsigned int tableSize /*= 2048*/)
{
	this->tableSize = tableSize;
	this->table = new LinkedList[tableSize];
	this->numNodes = 0;
}

/*virtual*/ HashTable::~HashTable()
{
	this->Clear();
	delete[] this->table;
}

HashTableNode* HashTable::FindNode(const HashTableKey* key)
{
	unsigned int i = key->Hash(this->tableSize);
	UU_ASSERT(i < this->tableSize);
	if (i >= this->tableSize)
		return nullptr;

	LinkedList* list = &this->table[i];
	LinkedListNode* node = list->GetHead();
	while (node)
	{
		if (*static_cast<HashTableNode*>(node)->key == *key)
			break;

		node = node->GetNext();
	}

	return static_cast<HashTableNode*>(node);
}

bool HashTable::InsertNode(HashTableNode* newNode)
{
	if (newNode->table != nullptr)
		return false;

	unsigned int i = newNode->key->Hash(this->tableSize);
	UU_ASSERT(i < this->tableSize);
	if (i >= this->tableSize)
		return false;

	LinkedList* list = &this->table[i];
	list->InsertAfter(newNode);
	newNode->table = this;
	this->numNodes++;
	return true;
}

HashTableNode* HashTable::RemoveNode(const HashTableKey* key)
{
	HashTableNode* oldNode = this->FindNode(key);
	if (!oldNode)
		return nullptr;

	oldNode->GetList()->Remove(oldNode);
	oldNode->table = nullptr;
	this->numNodes--;
	return oldNode;
}

bool HashTable::RemoveNode(HashTableNode* oldNode)
{
	if (oldNode->table != this)
		return false;

	oldNode->GetList()->Remove(oldNode);
	oldNode->table = nullptr;
	this->numNodes--;
	return true;
}

bool HashTable::DeleteNode(HashTableKey* key)
{
	HashTableNode* oldNode = this->RemoveNode(key);
	if (!oldNode)
		return false;

	delete oldNode;
	return true;
}

void HashTable::Clear()
{
	for (unsigned int i = 0; i < this->tableSize; i++)
		this->table[i].Clear();

	this->numNodes = 0;
}

//------------------------------ HashTableNode ------------------------------

HashTableNode::HashTableNode()
{
	this->key = nullptr;
	this->table = nullptr;
}

/*virtual*/ HashTableNode::~HashTableNode()
{
}

void HashTableNode::SetKey(HashTableKey* key)
{
	this->key = key;
}

HashTableKey* HashTableNode::GetKey()
{
	return this->key;
}