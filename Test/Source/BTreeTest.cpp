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
	BTree tree(8);

	SECTION("Test tree key insertion.")
	{
		int j = 1000;
		for (int i = 0; i < j; i++)
		{
			tree.InsertKey(new Key(i));
			REQUIRE(tree.IsBalanced());
			REQUIRE(tree.DegreesValid());
			REQUIRE(tree.GetNumKeys() == i + 1);
		}
	}

	SECTION("Test tree key removal.")
	{
		int j = 1000;
		for (int i = 0; i < j; i++)
			tree.InsertKey(new Key(i));

		for (int i = 0; i < j; i++)
		{
			Key key(i);
			tree.RemoveKey(&key);
			REQUIRE(tree.IsBalanced());
			REQUIRE(tree.DegreesValid());
			REQUIRE(tree.GetNumKeys() == j - i - 1);
		}
	}
}