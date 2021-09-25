project(Gasyori100knock_c C)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CFLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -Wextra -Wpedantic")

add_subdirectory(example)
add_subdirectory(utils)
add_subdirectory(questions)
