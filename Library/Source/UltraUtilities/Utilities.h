#pragma once

#if defined ULTRA_UTILITIES_EXPORT
#	define UU_API		__declspec(dllexport)
#elif defined ULTRA_UTILITIES_IMPORT
#	define UU_API		__declspec(dllimport)
#else
#	define UU_API
#endif