#pragma once

#include "UltraUtilities/Containers/RBTree.h"
#include "UltraUtilities/Memory/ObjectHeap.hpp"

namespace UU
{
	template<typename K> class RBMapKey;
	template<typename V> class RBMapNode;
	template<typename K, typename V> class RBMapIterator;

	/**
	 * This is a templatized wrapper for the @ref RBTree class.
	 * It is provided as a more convenient way to use the tree.
	 */
	template<typename K, typename V, typename NH = DefaultObjectHeap<RBMapNode<V>>, typename KH = DefaultObjectHeap<RBMapKey<K>>>
	class UU_API RBMap
	{
	public:
		RBMap()
		{
			this->iterationDirection = RBMapIterator<K, V>::FORWARD;
		}

		/**
		 * This provides bracket-syntax access to the map.
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
		 * then this method can be used to check for existance
		 * of the given key in the tree.
		 */
		bool Find(K key, V* value = nullptr)
		{
			RBMapKey<K> mapKey;
			mapKey.value = key;
			auto node = static_cast<RBMapNode<V>*>(this->tree.FindNode(&mapKey));
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
			RBMapKey<K> mapKey;
			mapKey.value = key;
			auto node = static_cast<RBMapNode<V>*>(this->tree.FindNode(&mapKey));
			if (node)
				node->value = value;
			else
			{
				node = this->nodeHeap.Allocate();
				node->value = value;
				node->SetKey(this->keyHeap.Allocate());
				static_cast<RBMapKey<K>*>(node->GetKey())->value = key;
				if (!this->tree.InsertNode(node))
				{
					this->keyHeap.Deallocate(static_cast<RBMapKey<K>*>(node->GetKey()));
					this->nodeHeap.Deallocate(static_cast<RBMapNode<V>*>(node));
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
			RBMapKey<K> mapKey;
			mapKey.value = key;
			RBTreeNode* node = this->tree.FindNode(&mapKey);
			if (!node)
				return false;
			if (value)
				*value = static_cast<RBMapNode<V>*>(node)->value;
			this->tree.RemoveNode(node);
			this->keyHeap.Deallocate(static_cast<RBMapKey<K>*>(node->GetKey()));
			this->nodeHeap.Deallocate(static_cast<RBMapNode<V>*>(node));
			return true;
		}

		/**
		 * Remove all key/value pairs.
		 */
		void Clear()
		{
			this->tree.Clear();
		}

		/**
		 * Indicate how many key/value pairs are stored in the tree.
		 */
		unsigned int GetNumPairs() const { return this->tree.GetNumNodes(); }

		/**
		 * Provide access to the tree being wrapped by this template class.
		 */
		RBTree& GetTree() { return this->tree; }

		/**
		 * This configures the behavior of the ranged for-loop syntax of C++ with regards to this container.
		 */
		void SetIterationDirection(RBMapIterator<K, V>::Direction iterationDirection) const
		{
			this->iterationDirection = iterationDirection;
		}

		/**
		 * This is provided to support the ranged for-loop syntax.
		 */
		RBMapIterator<K, V> begin()
		{
			return RBMapIterator<K, V>(&this->tree, this->iterationDirection);
		}

		/**
		 * This is the end sentinal for the ranged for-loop support.
		 */
		RBTreeNode* end()
		{
			return nullptr;
		}

	private:
		RBTree tree;
		mutable RBMapIterator<K, V>::Direction iterationDirection;
		NH nodeHeap;
		KH keyHeap;
	};

	/**
	 * This is used internally by the @ref RBMap class to
	 * support the ranged for-loop syntax of C++.
	 */
	template<typename K, typename V>
	class UU_API RBMapIterator
	{
	public:
		enum Direction
		{
			FORWARD,
			BACKWARD
		};

		struct Pair
		{
			K key;
			V value;
		};

		RBMapIterator(RBTree* tree, Direction direction)
		{
			this->direction = direction;
			switch (this->direction)
			{
			case FORWARD:
				this->node = tree->FindMinimum();
				break;
			case BACKWARD:
				this->node = tree->FindMaximum();
				break;
			default:
				this->node = nullptr;
				break;
			}
		}

		void operator++()
		{
			switch (this->direction)
			{
			case FORWARD:
				this->node = this->node->FindSuccessor();
				break;
			case BACKWARD:
				this->node = this->node->FindPredecessor();
				break;
			}
		}

		bool operator==(RBTreeNode* node)
		{
			return this->node == node;
		}

		Pair operator*()
		{
			Pair pair;
			pair.key = static_cast<const RBMapKey<K>*>(this->node->GetKey())->value;
			pair.value = static_cast<RBMapNode<V>*>(this->node)->value;
			return pair;
		}

	private:
		RBTreeNode* node;
		Direction direction;
	};

	/**
	 * This is used internally by the @ref RBMap class.
	 */
	template<typename V>
	class UU_API RBMapNode : public RBTreeNode
	{
	public:
		RBMapNode()
		{
			// Note that the value is left uninitialized!
		}

		virtual ~RBMapNode()
		{
		}

		virtual void CopyValue(RBTreeNode* node) override
		{
			this->value = static_cast<RBMapNode<V>*>(node)->value;
		}

	public:
		V value;
	};

	/**
	 * This is used internally by the @ref RBMap class.
	 */
	template<typename K>
	class UU_API RBMapKey : public RBTreeKey
	{
	public:
		RBMapKey()
		{
			// Note that the key value is left uninitialized!
		}

		virtual ~RBMapKey()
		{
		}

		virtual bool operator<(const RBTreeKey& key) const override
		{
			return this->value < static_cast<const RBMapKey<K>*>(&key)->value;
		}

		virtual bool operator>(const RBTreeKey& key) const override
		{
			return this->value > static_cast<const RBMapKey<K>*>(&key)->value;
		}

		virtual bool operator==(const RBTreeKey& key) const override
		{
			return this->value == static_cast<const RBMapKey<K>*>(&key)->value;
		}

	public:
		K value;
	};
}