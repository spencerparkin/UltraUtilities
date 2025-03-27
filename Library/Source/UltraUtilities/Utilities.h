#pragma once

#if defined ULTRA_UTILITIES_EXPORT
#	define UU_API		__declspec(dllexport)
#elif defined ULTRA_UTILITIES_IMPORT
#	define UU_API		__declspec(dllimport)
#else
#	define UU_API
#endif

#include <assert.h>
#include <functional>

// TODO: Remove all dependence on std::c++?
// TODO: Add priority queue.
// TODO: Add hash map.
// TODO: Add link-list and thread-safe stuff.
// TODO: Add string class?
// TODO: Add custom assert functionality.