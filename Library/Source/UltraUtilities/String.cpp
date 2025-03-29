#include "UltraUtilities/String.h"

using namespace UU;

String::String()
{
	this->charArray = new DArray<char>();
	this->charArray->Push('\0');
}

String::String(const String& string)
{
	this->charArray = new DArray<char>(string.Length() + 1);
	this->charArray->SetSize(string.Length() + 1);
	for (unsigned int i = 0; i < string.charArray->GetSize(); i++)
		(*this->charArray)[i] = (*string.charArray)[i];
}

String::String(String&& string)
{
	this->charArray = string.charArray;
	string.charArray = nullptr;
}

String::String(const char* string)
{
	unsigned int length = 0;
	while (string[length] != '\0')
		length++;
	this->charArray = new DArray<char>(length + 1);
	for (unsigned int i = 0; i < length + 1; i++)
		(*this->charArray)[i] = string[i];
}

String::String(const String& stringA, const String& stringB)
{
	this->charArray = new DArray<char>(stringA.Length() + stringB.Length() + 1);
	int j = 0;
	for (unsigned int i = 0; i < stringA.Length(); i++)
		(*this->charArray)[j++] = stringA[i];
	for (unsigned int i = 0; i < stringB.Length(); i++)
		(*this->charArray)[j++] = stringB[i];
	(*this->charArray)[j] = '\0';
}

String::String(const String& stringA, const char* stringB)
{
	unsigned int lengthB = 0;
	while (stringB[lengthB] != '\0')
		lengthB++;
	this->charArray = new DArray<char>(stringA.Length() + lengthB + 1);
	int j = 0;
	for (unsigned int i = 0; i < stringA.Length(); i++)
		(*this->charArray)[j++] = stringA[i];
	for (unsigned int i = 0; i < lengthB; i++)
		(*this->charArray)[j++] = stringB[i];
	(*this->charArray)[j] = '\0';
}

String::String(const char* stringA, const String& stringB)
{
	unsigned int lengthA = 0;
	while (stringA[lengthA] != '\0')
		lengthA++;
	this->charArray = new DArray<char>(lengthA + stringB.Length() + 1);
	int j = 0;
	for (unsigned int i = 0; i < lengthA; i++)
		(*this->charArray)[j++] = stringA[i];
	for (unsigned int i = 0; i < stringB.Length(); i++)
		(*this->charArray)[j++] = stringB[i];
	(*this->charArray)[j] = '\0';
}

/*virtual*/ String::~String()
{
	delete this->charArray;
}

void String::operator=(const String& string)
{
	this->charArray->EnsureCapacity(string.Length() + 1);
	this->charArray->SetSize(string.Length() + 1);
	for (unsigned int i = 0; i < string.charArray->GetSize(); i++)
		(*this->charArray)[i] = (*string.charArray)[i];
}

void String::operator=(String&& string)
{
	this->charArray = string.charArray;
	string.charArray = nullptr;
}

void String::operator=(const char* string)
{
	unsigned int length = 0;
	while (string[length] != '\0')
		length++;
	this->charArray->EnsureCapacity(length + 1);
	this->charArray->SetSize(length + 1);
	for (unsigned int i = 0; i < length + 1; i++)
		(*this->charArray)[i] = string[i];
}

int String::CompareWith(const String& string) const
{
	unsigned int lengthA = this->Length();
	unsigned int lengthB = string.Length();

	unsigned int j = lengthA < lengthB ? lengthA : lengthB;

	for (unsigned int i = 0; i < j; i++)
	{
		char charA = (i < lengthA) ? (*this->charArray)[i] : 0;
		char charB = (i < lengthB) ? (*string.charArray)[i] : 0;

		if (charA < charB)
			return -1;
		if (charA > charB)
			return 1;
	}

	return 0;
}