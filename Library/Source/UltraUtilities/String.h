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

		int CompareWith(const String& string) const;

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