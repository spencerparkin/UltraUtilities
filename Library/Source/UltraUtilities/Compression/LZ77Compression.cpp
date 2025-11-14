#include "UltraUtilities/Compression/LZ77Compression.h"
#include "UltraUtilities/Containers/DArray.hpp"

using namespace UU;

LZ77Compression::LZ77Compression(unsigned int windowSize)
{
	this->windowSize = windowSize;
}

/*virtual*/ LZ77Compression::~LZ77Compression()
{
}

/*virtual*/ bool LZ77Compression::Compress(ByteStream* inputStream, ByteStream* outputStream)
{
	if (inputStream == outputStream)
		return false;

	const char* inputBuffer = inputStream->GetBuffer();
	if (!inputBuffer)
		return false;

	unsigned int inputBufferSize = inputStream->GetSize();
	if (inputBufferSize == 0)
		return false;

	unsigned int currentPosition = 0;

	if (outputStream->WriteBytes((const char*)&this->windowSize, sizeof(this->windowSize)) != sizeof(this->windowSize))
		return false;

	while (currentPosition < inputBufferSize)
	{
		unsigned int currentWindowSize = UU_MIN(currentPosition, this->windowSize);

		Packet packet{ 0, 0, 0 };

		for (unsigned int i = 0; i < currentWindowSize; i++)
		{
			unsigned int j;
			for (j = 0; j < currentWindowSize - i && currentPosition + j < inputBufferSize - 1; j++)
				if (inputBuffer[currentPosition - currentWindowSize + i + j] != inputBuffer[currentPosition + j])
					break;
			
			if (j > packet.length)
			{
				packet.length = j;
				packet.offset = currentWindowSize - i;
				packet.byte = inputBuffer[currentPosition + j];
			}
		}

		if (packet.length == 0)
			packet.byte = inputBuffer[currentPosition];

		if (outputStream->WriteBytes((const char*)&packet, sizeof(packet)) != sizeof(packet))
			return false;

		currentPosition += packet.length + 1;
	}

	return true;
}

/*virtual*/ bool LZ77Compression::Decompress(ByteStream* inputStream, ByteStream* outputStream)
{
	if (inputStream->ReadBytes((char*)&this->windowSize, sizeof(this->windowSize)) != sizeof(this->windowSize))
		return false;

	DArray<char> outputBuffer;
	unsigned int currentPosition = 0;

	while (true)
	{
		Packet packet;
		if (inputStream->ReadBytes((char*)&packet, sizeof(packet)) != sizeof(packet))
			break;

		if (currentPosition < packet.offset)
			return false;

		outputBuffer.SetSize(outputBuffer.GetSize() + packet.length + 1);

		unsigned int i;
		for (i = 0; i < packet.length; i++)
		{
			unsigned int j = currentPosition - packet.offset + i;
			if (j >= currentPosition)
				return false;

			outputBuffer[currentPosition + i] = outputBuffer[j];
		}

		outputBuffer[currentPosition + i] = packet.byte;
		currentPosition += i + 1;
	}

	if (outputStream->WriteBytes(outputBuffer.GetBuffer(), outputBuffer.GetSize()) != outputBuffer.GetSize())
		return false;

	return true;
}