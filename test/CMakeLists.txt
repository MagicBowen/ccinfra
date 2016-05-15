project(ccinfra-test)

FILE(GLOB_RECURSE all_files
*.cpp
*.cc
*.c++
*.c
*.C)

add_executable(ccinfra-test  ${all_files})
target_link_libraries(ccinfra-test gtest ccinfra)
