#include "UltraUtilities/String.h"
#include "UltraUtilities/Containers/RBSet.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace UU;

TEST_CASE("Dynamic Strings", "[string]")
{
	String stringA;
	REQUIRE(stringA.Length() == 0);

	SECTION("Test construction and assignment.")
	{
		stringA = "Hello";
		REQUIRE(stringA.Length() == 5);
		REQUIRE(stringA[0] == 'H');
		REQUIRE(stringA[1] == 'e');
		REQUIRE(stringA[2] == 'l');
		REQUIRE(stringA[3] == 'l');
		REQUIRE(stringA[4] == 'o');

		String stringB(stringA);
		REQUIRE(stringB.Length() == 5);
		REQUIRE(stringB[0] == 'H');
		REQUIRE(stringB[1] == 'e');
		REQUIRE(stringB[2] == 'l');
		REQUIRE(stringB[3] == 'l');
		REQUIRE(stringB[4] == 'o');

		String stringC = String("Good-bye!");
		REQUIRE(stringC.Length() == 9);
		REQUIRE(stringC[0] == 'G');
		REQUIRE(stringC[8] == '!');
	}
	
	SECTION("Test concatination and comparison.")
	{
		stringA = "Crunchy";
		String stringB = "toast!";
		String stringC = stringA + " " + stringB;
		String stringD("Crunchy toast!");
		String stringE = stringA + stringB;
		bool result = stringD == stringC;
		REQUIRE(stringD == stringC);
		REQUIRE(stringD != stringE);
		REQUIRE(stringD.Length() == stringE.Length() + 1);
	}

	SECTION("String ordering.")
	{
		stringA = "Apple";
		String stringB = "Blueberry";
		String stringC = String("Custard");
		String stringD("Donut");

		RBSet<String> foodSet;
		foodSet.Insert(stringD);
		foodSet.Insert(stringC);
		foodSet.Insert(stringB);
		foodSet.Insert(stringA);

		int i = 0;
		for (const String& food : foodSet)
		{
			switch (i++)
			{
			case 0:
				REQUIRE(food == "Apple");
				break;
			case 1:
				REQUIRE(food == "Blueberry");
				break;
			case 2:
				REQUIRE(food == "Custard");
				break;
			case 3:
				REQUIRE(food == "Donut");
				break;
			}
		}
	}
}