# CMakeLists.txt for the UltraUtilities library.

set(ULTRA_UTILITIES_SOURCES
	Source/UltraUtilities/Defines.h
	Source/UltraUtilities/Containers/RBTree.cpp
	Source/UltraUtilities/Containers/RBTree.h
	Source/UltraUtilities/Containers/RBMap.hpp
	Source/UltraUtilities/Containers/RBSet.hpp
	Source/UltraUtilities/Containers/LinkedList.cpp
	Source/UltraUtilities/Containers/LinkedList.h
	Source/UltraUtilities/Containers/List.hpp
)

add_library(UltraUtilities SHARED
	${ULTRA_UTILITIES_SOURCES}
)

target_compile_definitions(UltraUtilities PRIVATE
	ULTRA_UTILITIES_EXPORT
)

target_include_directories(UltraUtilities PUBLIC
	Source
)