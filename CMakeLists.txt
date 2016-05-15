cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME "ccinfra")

project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof -g -std=c++11")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory("src")

install(DIRECTORY include/ccinfra DESTINATION include)
