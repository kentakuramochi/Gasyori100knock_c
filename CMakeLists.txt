project(Gasyori100knock_c C)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER gcc)

add_compile_options(-std=c11)

add_subdirectory(utils)
add_subdirectory(questions)
add_subdirectory(example)
