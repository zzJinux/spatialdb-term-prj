cmake_minimum_required(VERSION 3.10)
project(term_project C)
enable_testing()

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)
add_subdirectory(test)
