# CMakeLists.txt for Catch2-based test program that exercizes/tests the UltraUtilities library.

set(TEST_SOURCES
	Sources/Test.cpp
	Sources/Test.h
)

add_executable(Test ${TEST_SOURCES})

target_link_libraries(Test PRIVATE
	UltraUtilities
	Catch2::Catch2WithMain
)