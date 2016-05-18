project(ccinfra-test)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}"
                                     "${GTEST_ROOT}/include")
                                     
link_directories("${GTEST_ROOT}/lib")                                     

FILE(GLOB_RECURSE all_files
*.cpp
*.cc
*.c++
*.c
*.C)

add_executable(ccinfra-test  ${all_files})
target_link_libraries(ccinfra-test gtest pthread ccinfra)
