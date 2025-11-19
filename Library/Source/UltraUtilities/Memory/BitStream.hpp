#pragma once

#include "UltraUtilities/Memory/ByteStream.h"

namespace UU
{
	/**
	 * This class sits on top of a byte stream and lets you read and write
	 * in terms of bits instead of bytes.  This can be useful if you're trying
	 * to read or write very compact data.
	 */
	class UU_API BitStream
	{
	public:
		BitStream(ByteStream* byteStream)
		{
			this->byteStream = byteStream;
			this->currentByteWrite = 0;
			this->bitOffsetWrite = 0;
			this->currentByteRead = 0;
			this->bitOffsetRead = 7;
		}

		virtual ~BitStream()
		{
		}

		/**
		 * Write the given number of bits in the given data to the byte stream.
		 * 
		 * @param[in] dataIn Bits are taken from this starting at the LSB and moving toward the MSB.
		 * @param[in] numBits This is the number of bits to write and must not exceed the number of bits available in the given data.
		 * @return Failure can occur here if the underlying byte stream fails to write.
		 */
		template<typename T>
		bool WriteBits(T dataIn, unsigned int numBits)
		{
			if (numBits / 8 >= sizeof(T))
				return false;

			for (unsigned int i = 0; i < numBits; i++)
			{
				if ((dataIn & (1 << i)) != 0)
					this->currentByteWrite |= (1 << this->bitOffsetWrite);

				if (++this->bitOffsetWrite == 8)
				{
					if (this->byteStream->WriteBytes((const char*)&this->currentByteWrite, 1) != 1)
						return false;

					this->bitOffsetWrite = 0;
					this->currentByteWrite = 0;
				}
			}

			return true;
		}

		/**
		 * This just calls @ref WriteBits with the given data and
		 * a number of bits equal to the capacity of that data.
		 */
		template<typename T>
		bool WriteAllBits(T dataIn)
		{
			return this->WriteBits(dataIn, sizeof(T) * 8);
		}

		/**
		 * Read the given number of bits into the given data from the byte stream.
		 * 
		 * @param[out] dataOut Bits are put into this starting at the LSB and moving toward the MSB.
		 * @param[in] numBits This is the number of bits to read and must not exceed the number of bits available in the given data.
		 * @return Failure can occur here if the underlying byte stream fails to read.
		 */
		template<typename T>
		bool ReadBits(T& dataOut, unsigned int numBits)
		{
			if (numBits / 8 >= sizeof(T))
				return false;

			for (unsigned int i = 0; i < numBits; i++)
			{
				if (++this->bitOffsetRead == 8)
				{
					if (this->byteStream->ReadBytes((char*)&this->currentByteRead, 1) != 1)
						return false;

					this->bitOffsetRead = 0;
				}

				if ((this->currentByteRead & (1 << this->bitOffsetRead)) != 0)
					dataOut |= (1 << i);
			}

			return true;
		}

		/**
		 * This just calls @ref ReadBits with the given data and
		 * a number of bits equal to the capacity of that data.
		 */
		template<typename T>
		bool ReadAllBits(T& dataOut)
		{
			return this->ReadBits(dataOut, sizeof(T) * 8);
		}

		/**
		 * Once you're done writing to the bit stream, you should call this
		 * to flush any pending bits that there may be in the current byte.
		 */
		bool Flush()
		{
			unsigned char data = 0;
			return this->WriteBits(data, 8 - this->bitOffsetWrite);
		}

	private:
		ByteStream* byteStream;
		unsigned int bitOffsetWrite;
		unsigned char currentByteWrite;
		unsigned int bitOffsetRead;
		unsigned char currentByteRead;
	};
}