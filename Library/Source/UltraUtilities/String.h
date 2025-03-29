#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Containers/DArray.hpp"

namespace UU
{
	/**
	 * These are null-terminated arrays of characters.
	 */
	class UU_API String
	{
	public:
		String();
		String(const String& string);
		String(String&& string);
		String(const char* string);
		String(const String& stringA, const String& stringB);
		String(const String& stringA, const char* stringB);
		String(const char* stringA, const String& stringB);
		virtual ~String();

		void operator=(const String& string);
		void operator=(String&& string);
		void operator=(const char* string);

		unsigned int Length() const
		{
			return this->charArray->GetSize() - 1;
		}

		operator const char*() const
		{
			return this->charArray->GetBuffer();
		}

		char operator[](unsigned int i) const
		{
			return (*this->charArray)[i];
		}

		/**
		 * Alphabetically compare this string with the given string.
		 */
		int CompareWith(const String& string) const;

		/**
		 * This is provided for compatabillity with the @ref HashSet and @ref HashMap
		 * classes so that string keys can be checked for equality.
		 */
		static bool Equal(const String& stringA, const String& stringB)
		{
			return stringA == stringB;
		}

		/**
		 * This is provided for compatabillity with the @ref HashSet and @ref HashMap
		 * classes so that string keys can be hashed.
		 */
		static unsigned int Hash(const String& string, unsigned int tableSize)
		{
			unsigned int hash = 0;
			for (unsigned int i = 0; i < string.Length(); i++)
			{
				auto j = (unsigned int)(string[i]);

				// I made this up and it's probably stupid.
				hash += j;
				hash ^= j << i;
				hash += hash << j;
				hash += hash << i * j;
			}
			return hash;
		}

		// TODO: Add split and combine, each with a delimeter.

	private:
		DArray<char>* charArray;
	};

	inline String operator+(const String& stringA, const String& stringB)
	{
		return String(stringA, stringB);
	}

	inline String operator+(const String& stringA, const char* stringB)
	{
		return String(stringA, stringB);
	}

	inline String operator+(const char* stringA, const String& stringB)
	{
		return String(stringA, stringB);
	}

	inline bool operator<(const String& stringA, const String& stringB)
	{
		return stringA.CompareWith(stringB) < 0;
	}

	inline bool operator>(const String& stringA, const String& stringB)
	{
		return stringA.CompareWith(stringB) > 0;
	}

	inline bool operator==(const String& stringA, const String& stringB)
	{
		return stringA.CompareWith(stringB) == 0;
	}

	inline bool operator==(const String& stringA, const char* stringB)
	{
		return stringA.CompareWith(String(stringB)) == 0;
	}

	inline bool operator==(const char* stringA, const String& stringB)
	{
		return String(stringA).CompareWith(stringB) == 0;
	}

	inline bool operator!=(const String& stringA, const String& stringB)
	{
		return stringA.CompareWith(stringB) != 0;
	}

	inline bool operator!=(const String& stringA, const char* stringB)
	{
		return stringA.CompareWith(String(stringB)) != 0;
	}

	inline bool operator!=(const char* stringA, const String& stringB)
	{
		return String(stringA).CompareWith(stringB) != 0;
	}
}