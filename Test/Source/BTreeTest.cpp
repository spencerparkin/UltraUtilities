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
	BTree tree(4);

	SECTION("Test tree key insertion.")
	{
		int numKeys = 1000;
		for (int i = 0; i < numKeys; i++)
		{
			tree.InsertKey(new Key(i));
			REQUIRE(tree.IsBalanced());
			REQUIRE(tree.DegreesValid());
			REQUIRE(tree.GetNumKeys() == i + 1);
			
			for (int j = 0; j <= i; j++)
			{
				Key key(j);
				REQUIRE(tree.FindKey(&key) != nullptr);
			}
		}
	}

	SECTION("Test tree key removal.")
	{
		int numKeys = 1000;
		for (int i = 0; i < numKeys; i++)
			tree.InsertKey(new Key(i));

		for (int i = 0; i < numKeys; i++)
		{
			Key key(i);
			tree.RemoveKey(&key);
			REQUIRE(tree.IsBalanced());
			REQUIRE(tree.DegreesValid());
			REQUIRE(tree.GetNumKeys() == numKeys - i - 1);
		}
	}
}