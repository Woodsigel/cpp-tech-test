cmake_minimum_required(VERSION 3.16)

add_library(graph graph.cpp)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(elaborated main.cpp)

target_link_libraries(elaborated 
		PRIVATE graph)