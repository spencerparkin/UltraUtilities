#pragma once

#include "UltraUtilities/Containers/RBMap.hpp"

namespace UU
{
	template<typename K> class RBSetIterator;

	/**
	 * This is a templatized wrapper for the @ref RBTree class.
	 * It is provided as a more convenient way to use the tree.
	 * Note that unlike @ref HashSet, this set imposes an ordering
	 * on its elements.
	 */
	template<typename K>
	class UU_API RBSet
	{
	public:
		RBSet()
		{
			this->iterationDirection = RBSetIterator<K>::FORWARD;
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
			auto node = new RBTreeNode();
			node->SetKey(new RBMapKey<K>(key));
			if (!this->tree.InsertNode(node))
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
			RBMapKey<K> treeKey(key);
			RBTreeNode* node = this->tree.FindNode(&treeKey);
			if (!node)
				return false;
			this->tree.RemoveNode(node);
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
		 * Provide access to the tree being wrapped by this template class.
		 */
		RBTree& GetTree() { return this->tree; }

		/**
		 * This configures the behavior of the ranged for-loop syntax of C++ with regards to this container.
		 */
		void SetIterationDirection(RBSetIterator<K>::Direction iterationDirection) const
		{
			this->iterationDirection = iterationDirection;
		}

		/**
		 * This is provided to support the ranged for-loop syntax.
		 */
		RBSetIterator<K> begin()
		{
			return RBSetIterator<K>(&this->tree, this->iterationDirection);
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
		mutable RBSetIterator<K>::Direction iterationDirection;
	};

	/**
	 * This is used internally by the @ref RBSet class to
	 * support the ranged for-loop syntax of C++.
	 */
	template<typename K>
	class UU_API RBSetIterator
	{
	public:
		enum Direction
		{
			FORWARD,
			BACKWARD
		};

		RBSetIterator(RBTree* tree, Direction direction)
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

		K operator*()
		{
			return static_cast<const RBMapKey<K>*>(this->node->GetKey())->value;
		}

	private:
		RBTreeNode* node;
		Direction direction;
	};
}