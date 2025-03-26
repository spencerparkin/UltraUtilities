#pragma once

#if defined ULTRA_UTILITIES_EXPORT
#	define ULTRA_UTILITIES_API		__declspec(dllexport)
#elif defined ULTRA_UTILITIES_IMPORT
#	define ULTRA_UTILITIES_API		__declspec(dllimport)
#else
#	define ULTRA_UTILITIES_API
#endif