# CMakeLists.txt for the UltraUtilities library.

set(ULTRA_UTILITIES_SOURCES
	Source/UltraUtilities/Utilities.h
	Source/UltraUtilities/Containers/RBTree.cpp
	Source/UltraUtilities/Containers/RBTree.h
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