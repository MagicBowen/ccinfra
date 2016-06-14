project(ccinfra-test)

include_directories(${MAGELLAN_INCLUDE_DIR})

FILE(GLOB_RECURSE all_files
*.cpp
*.cc
*.c++
*.c
*.C)

add_executable(ccinfra-test ${all_files})
target_link_libraries(ccinfra-test hamcrest magellan ccinfra)