# CMakeLists.txt for the UltraUtilities library.

set(ULTRA_UTILITIES_SOURCES
	Source/UltraUtilities/Defines.h
	Source/UltraUtilities/String.cpp
	Source/UltraUtilities/String.h
	Source/UltraUtilities/Graph.cpp
	Source/UltraUtilities/Graph.h
	Source/UltraUtilities/MulticastDelegate.hpp
	Source/UltraUtilities/DisjointSetForest.cpp
	Source/UltraUtilities/DisjointSetForest.h
	Source/UltraUtilities/Memory/ObjectHeap.hpp
	Source/UltraUtilities/Memory/Pointer.hpp
	Source/UltraUtilities/Memory/ByteStream.cpp
	Source/UltraUtilities/Memory/ByteStream.h
	Source/UltraUtilities/Containers/BTree.cpp
	Source/UltraUtilities/Containers/BTree.h
	Source/UltraUtilities/Containers/RBTree.cpp
	Source/UltraUtilities/Containers/RBTree.h
	Source/UltraUtilities/Containers/RBMap.hpp
	Source/UltraUtilities/Containers/RBSet.hpp
	Source/UltraUtilities/Containers/LinkedList.cpp
	Source/UltraUtilities/Containers/LinkedList.h
	Source/UltraUtilities/Containers/List.hpp
	Source/UltraUtilities/Containers/DArray.hpp
	Source/UltraUtilities/Containers/HashTable.cpp
	Source/UltraUtilities/Containers/HashTable.h
	Source/UltraUtilities/Containers/HashMap.hpp
	Source/UltraUtilities/Containers/HashSet.hpp
	Source/UltraUtilities/Containers/PriorityQueue.hpp
	Source/UltraUtilities/Compression/HuffmanCode.cpp
	Source/UltraUtilities/Compression/HuffmanCode.h
	uu.natvis
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