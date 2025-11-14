#pragma once

#include "UltraUtilities/Defines.h"
#include "UltraUtilities/Memory/ByteStream.h"

namespace UU
{
	/**
	 * This is the base class for any supported compression scheme.
	 */
	class UU_API Compression
	{
	public:
		Compression();
		virtual ~Compression();

		/**
		 * Compress the given input stream into the given output stream.
		 *
		 * @param[in] inputStream This is the data to compress.
		 * @param[out] outputStream This will hold the compression result.
		 * @return True is returned on success; false, otherwise.
		 */
		virtual bool Compress(ByteStream* inputStream, ByteStream* outputStream) = 0;

		/**
		 * Decompress the given input stream into the given output stream.
		 *
		 * @param[in] inputStream This is the data to decompress.
		 * @param[out] outputStream This will hold the decompression result.
		 * @return True is returned on success; false, otherwise.
		 */
		virtual bool Decompress(ByteStream* inputStream, ByteStream* outputStream) = 0;
	};
}