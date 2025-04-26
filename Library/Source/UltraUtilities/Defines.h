#pragma once

#if defined ULTRA_UTILITIES_EXPORT
#	define UU_API		__declspec(dllexport)
#elif defined ULTRA_UTILITIES_IMPORT
#	define UU_API		__declspec(dllimport)
#else
#	define UU_API
#endif

#if defined _WIN32
#	define WIN32_LEAN_AND_MEAN
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
#	define UU_UU_ASSERT(condition)
#endif

// TODO: Generate doxegen stuff?