#include "UltraUtilities/Containers/BTree.h"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

class Key : public BTreeKey
{
public:
	Key(int value)
	{
		this->value = value;
	}

	virtual ~Key()
	{
	}

	virtual bool IsEqualTo(const BTreeKey* key) const override
	{
		return this->value == static_cast<const Key*>(key)->value;
	}

	virtual bool IsLessThan(const BTreeKey* key) const override
	{
		return this->value < static_cast<const Key*>(key)->value;
	}

	virtual bool IsGreaterThan(const BTreeKey* key) const override
	{
		return this->value > static_cast<const Key*>(key)->value;
	}

public:
	int value;
};

TEST_CASE("B-Trees", "[btree]")
{
	BTree tree;

	SECTION("Test tree key insertion.")
	{
		int j = 100;
		for (int i = 0; i < j; i++)
		{
			tree.InsertKey(new Key(i));
			REQUIRE(tree.IsBalanced());
			REQUIRE(tree.DegreesValid());
		}
	}

	SECTION("Test tree key removal.")
	{
		// TODO: write this.
		REQUIRE(true);
	}
}