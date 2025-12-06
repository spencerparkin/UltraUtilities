#pragma once

#if defined ULTRA_UTILITIES_EXPORT
#	define UU_API		__declspec(dllexport)
#elif defined ULTRA_UTILITIES_IMPORT
#	define UU_API		__declspec(dllimport)
#else
#	define UU_API
#endif

#if defined _WIN32
#	if !defined WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <Windows.h>
#	include <debugapi.h>
#	define UU_ASSERT(condition) \
						do { \
							if (!(condition)) { \
								if (IsDebuggerPresent()) \
									DebugBreak(); \
							} \
						} while(false)
#else
#	define UU_ASSERT(condition)
#endif

#define UU_MIN(a,b)			((a) < (b) ? (a) : (b))
#define UU_MAX(a,b)			((a) > (b) ? (a) : (b))

namespace UU
{
	template<typename T>
	inline void Exchange(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}
}

// TODO: Generate doxegen stuff?