project(Gasyori100knock_c C)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER gcc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

set(LIB_NAME imgdata)

add_subdirectory(${LIB_NAME}/src)
add_subdirectory(answers)
add_subdirectory(example)
