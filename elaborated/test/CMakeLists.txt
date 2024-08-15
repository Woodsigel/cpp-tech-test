cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git 
	GIT_TAG       release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

get_target_property(graph_include_dir graph INCLUDE_DIRECTORIES)


add_executable(elaborated_test elaborated_test.cpp)

target_include_directories(elaborated_test
						PRIVATE
						${graph_include_dir})
target_link_libraries(elaborated_test
		PRIVATE
		GTest::GTest 
        graph)


add_test(NAME elaborated_test
		COMMAND elaborated_test)