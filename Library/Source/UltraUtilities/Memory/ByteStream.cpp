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

//---------------------------------- MemoryBufferStream ----------------------------------

MemoryBufferStream::MemoryBufferStream(char* memoryBuffer, unsigned int memoryBufferSize)
{
	this->memoryBuffer = memoryBuffer;
	this->memoryBufferSize = memoryBufferSize;
	this->readOffset = 0;
	this->writeOffset = 0;
}

/*virtual*/ MemoryBufferStream::~MemoryBufferStream()
{
}

/*virtual*/ unsigned int MemoryBufferStream::WriteBytes(const char* buffer, unsigned int bufferSize)
{
	unsigned int numBytesCanBeWritten = this->memoryBufferSize - this->writeOffset;
	unsigned int numBytesWritten = bufferSize <= numBytesCanBeWritten ? bufferSize : numBytesCanBeWritten;

	for (unsigned int i = 0; i < numBytesWritten; i++)
		this->memoryBuffer[this->writeOffset + i] = buffer[i];

	this->writeOffset += numBytesWritten;
	return numBytesWritten;
}

/*virtual*/ unsigned int MemoryBufferStream::ReadBytes(char* buffer, unsigned int bufferSize)
{
	unsigned int numBytesCanBeRead = this->GetSize();
	unsigned int numBytesRead = bufferSize <= numBytesCanBeRead ? bufferSize : numBytesCanBeRead;

	for (unsigned int i = 0; i < numBytesRead; i++)
		buffer[i] = this->memoryBuffer[this->readOffset + i];

	this->readOffset += numBytesRead;
	return numBytesRead;
}

/*virtual*/ unsigned int MemoryBufferStream::GetSize()
{
	return this->writeOffset - this->readOffset;
}

/*virtual*/ const char* MemoryBufferStream::GetBuffer() const
{
	return &this->memoryBuffer[this->readOffset];
}

//---------------------------------- RingBufferStream ----------------------------------

RingBufferStream::RingBufferStream(unsigned int size)
{
	this->ringBufferSize = size;
	this->ringBuffer = new char[size];
	this->readOffset = 0;
	this->writeOffset = 0;
}

/*virtual*/ RingBufferStream::~RingBufferStream()
{
	delete[] this->ringBuffer;
}

/*virtual*/ unsigned int RingBufferStream::WriteBytes(const char* buffer, unsigned int bufferSize)
{
	unsigned int numBytesWritten = 0;

	while (numBytesWritten < bufferSize)
	{
		unsigned int numBytesFree = this->ringBufferSize - this->GetSize();
		if (numBytesFree == 0)
			break;

		this->ringBuffer[this->writeOffset] = buffer[numBytesWritten++];
		this->writeOffset = (this->writeOffset + 1) % this->ringBufferSize;
	}

	return numBytesWritten;
}

/*virtual*/ unsigned int RingBufferStream::ReadBytes(char* buffer, unsigned int bufferSize)
{
	unsigned int numBytesRead = 0;

	while (numBytesRead < bufferSize)
	{
		unsigned int numBytesUsed = this->GetSize();
		if (numBytesUsed == 0)
			break;

		buffer[numBytesRead++] = this->ringBuffer[this->readOffset];
		this->readOffset = (this->readOffset + 1) % this->ringBufferSize;
	}

	return numBytesRead;
}

/*virtual*/ unsigned int RingBufferStream::GetSize()
{
	if (this->readOffset < this->writeOffset)
		return this->writeOffset - this->readOffset;

	return this->ringBufferSize - this->readOffset + this->writeOffset;
}

/*virtual*/ const char* RingBufferStream::GetBuffer() const
{
	return nullptr;
}