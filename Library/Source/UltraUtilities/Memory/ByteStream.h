#pragma once

#include "UltraUtilities/Defines.h"

namespace UU
{
	/**
	 * This is the base class for any type of byte stream.  It could be an in-memory stream,
	 * a network stream, a file stream, etc.  They can be input-only, output-only, bidirectional.
	 */
	class UU_API ByteStream
	{
	public:
		ByteStream();
		virtual ~ByteStream();

		/**
		 * Write data to this tream.
		 * 
		 * @param[in] buffer This is the data to write.
		 * @param[in] bufferSize This is the size of the given buffer.
		 * @return The number of contiguous bytes written to this stream and read from the given buffer should be returned.
		 */
		virtual unsigned int WriteBytes(const char* buffer, unsigned int bufferSize) = 0;

		/**
		 * Read ata from this stream.
		 * 
		 * @param[out] buffer Data form the stream is written here.
		 * @param[in] bufferSize This is the size of the given buffer.
		 * @return The number of contiguous bytes read from this stream and written to the given buffer should be returned.
		 */
		virtual unsigned int ReadBytes(char* buffer, unsigned int bufferSize) = 0;

		/**
		 * Return the amount of memory in the stream in bytes.
		 * This doesn't have to be supported by the stream since
		 * some streams are essentially of infinite size or are of
		 * an indeterminant size.
		 */
		virtual unsigned int GetSize() = 0;

		/**
		 * Return the bytes of the stream in one contiguous buffer.
		 * This does not have to be supported by the stream.
		 */
		virtual const char* GetBuffer() const;
	};

	/**
	 * Provide a convenient way to read/write from/to an in-memory buffer.  Note that
	 * we do not take ownership of the buffer here.
	 */
	class UU_API MemoryBufferStream : public ByteStream
	{
	public:
		MemoryBufferStream(char* memoryBuffer, unsigned int memoryBufferSize);
		virtual ~MemoryBufferStream();

		virtual unsigned int WriteBytes(const char* buffer, unsigned int bufferSize) override;
		virtual unsigned int ReadBytes(char* buffer, unsigned int bufferSize) override;
		virtual unsigned int GetSize() override;
		virtual const char* GetBuffer() const override;

	private:
		char* memoryBuffer;
		unsigned int memoryBufferSize;
		unsigned int readOffset;
		unsigned int writeOffset;
	};

	/**
	 * This class provides a buffer to which you can perpetually write,
	 * and from which you can perpetually read, provided the amount of
	 * data in the stream at any one given time never goes over a pre-
	 * determined size limit.
	 */
	class UU_API RingBufferStream : public ByteStream
	{
	public:
		RingBufferStream(unsigned int size);
		virtual ~RingBufferStream();

		virtual unsigned int WriteBytes(const char* buffer, unsigned int bufferSize) override;
		virtual unsigned int ReadBytes(char* buffer, unsigned int bufferSize) override;
		virtual unsigned int GetSize() override;
		virtual const char* GetBuffer() const override;

	private:
		char* ringBuffer;
		unsigned int ringBufferSize;
		unsigned int readOffset;
		unsigned int writeOffset;
	};
}