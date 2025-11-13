# CMakeLists.txt for Catch2-based test program that exercizes/tests the UltraUtilities library.

set(TEST_SOURCES
	Source/RBMapTest.cpp
	Source/RBSetTest.cpp
	Source/ListTest.cpp
	Source/DArrayTest.cpp
	Source/StringTest.cpp
	Source/PriorityQueueTest.cpp
	Source/GraphTest.cpp
	Source/HashMapTest.cpp
	Source/HashSetTest.cpp
	Source/BTreeTest.cpp
	Source/HuffmanTest.cpp
)

add_executable(Test ${TEST_SOURCES})

target_link_libraries(Test PRIVATE
	UltraUtilities
	Catch2::Catch2WithMain
)