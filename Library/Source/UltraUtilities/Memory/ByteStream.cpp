#include "UltraUtilities/Memory/ByteStream.h"

using namespace UU;

//---------------------------------- ByteStream ----------------------------------

ByteStream::ByteStream()
{
}

/*virtual*/ ByteStream::~ByteStream()
{
}

/*virtual*/ const char* ByteStream::GetBuffer() const
{
	return nullptr;
}

//---------------------------------- MemoryStream ----------------------------------

MemoryStream::MemoryStream(char* memoryBuffer, unsigned int memoryBufferSize)
{
	this->memoryBuffer = memoryBuffer;
	this->memoryBufferSize = memoryBufferSize;
	this->readOffset = 0;
	this->writeOffset = 0;
}

/*virtual*/ MemoryStream::~MemoryStream()
{
}

/*virtual*/ unsigned int MemoryStream::WriteBytes(const char* buffer, unsigned int bufferSize)
{
	unsigned int numBytesCanBeWritten = this->memoryBufferSize - this->writeOffset;
	unsigned int numBytesWritten = bufferSize <= numBytesCanBeWritten ? bufferSize : numBytesCanBeWritten;

	for (unsigned int i = 0; i < numBytesWritten; i++)
		this->memoryBuffer[this->writeOffset + i] = buffer[i];

	this->writeOffset += numBytesWritten;
	return numBytesWritten;
}

/*virtual*/ unsigned int MemoryStream::ReadBytes(char* buffer, unsigned int bufferSize)
{
	unsigned int numBytesCanBeRead = this->GetSize();
	unsigned int numBytesRead = bufferSize <= numBytesCanBeRead ? bufferSize : numBytesCanBeRead;

	for (unsigned int i = 0; i < numBytesRead; i++)
		buffer[i] = this->memoryBuffer[this->readOffset + i];

	this->readOffset += numBytesRead;
	return numBytesRead;
}

/*virtual*/ unsigned int MemoryStream::GetSize()
{
	return this->writeOffset - this->readOffset;
}

/*virtual*/ const char* MemoryStream::GetBuffer() const
{
	return &this->memoryBuffer[this->readOffset];
}