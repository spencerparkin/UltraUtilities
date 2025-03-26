#pragma once

#include "UltraUtilities/Utilities.h"

namespace UU
{
	class RBTreeNodeBase;
	template<typename T> class RBTreeNode;

	/**
	 *
	 */
	class ULTRA_UTILITIES_API RBTreeBase
	{
	public:
		RBTreeBase();
		virtual ~RBTreeBase();

		virtual RBTreeNodeBase* NewNode() = 0;
	};

	/**
	 *
	 */
	template<typename T>
	class ULTRA_UTILITIES_API RBTree : public RBTreeBase
	{
	public:
		RBTree()
		{
		}

		virtual ~RBTree()
		{
		}

		virtual RBTreeNodeBase* NewNode() override
		{
			return new RBTreeNode<T>();
		}
	};

	/**
	 *
	 */
	class ULTRA_UTILITIES_API RBTreeNodeBase
	{
		friend class RBTreeBase;

	public:
		RBTreeNodeBase();
		virtual ~RBTreeNodeBase();

		virtual long Key() const;
		virtual bool operator<(const RBTreeNodeBase* node) const;
		virtual bool operator>(const RBTreeNodeBase* node) const;
		virtual bool operator==(const RBTreeNodeBase* node) const;

	private:
		RBTreeNodeBase* leftChildNode;
		RBTreeNodeBase* rightChildNode;
		RBTreeNodeBase* parentNode;
	};

	/**
	 *
	 */
	template<typename T>
	class ULTRA_UTILITIES_API RBTreeNode : public RBTreeNodeBase
	{
	public:
		RBTreeNode()
		{
		}

		virtual ~RBTreeNode()
		{
		}

		virtual bool operator<(const RBTreeNodeBase* node) const override
		{
			return this->data < static_cast<const RBTreeNode<T>*>(node)->data;
		}

		virtual bool operator>(const RBTreeNodeBase* node) const override
		{
			return this->data > static_cast<const RBTreeNode<T>*>(node)->data;
		}

		virtual bool operator==(const RBTreeNodeBase* node) const override
		{
			return this->data == static_cast<const RBTreeNode<T>*>(node)->data;
		}

	public:
		T data;
	};
}