file(GLOB FILES "./src/*.c")

add_library(${LIB_NAME} EXCLUDE_FROM_ALL SHARED
    ${FILES}
)

# Omit warning options for a situation that `argc`/`argv` is not used
# target_compile_options(${LIB_NAME} PRIVATE
#     PUBLIC -Wall -Wextra -Wpedantic -Werror
# )

target_include_directories(${LIB_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/${LIB_NAME}/include
)

target_link_libraries(${LIB_NAME} PRIVATE png)

set(EXAMPLE img_example)

add_executable(${EXAMPLE} EXCLUDE_FROM_ALL
    ${PROJECT_SOURCE_DIR}/${LIB_NAME}/example/example.c
)

target_link_libraries(${EXAMPLE} PRIVATE ${LIB_NAME})
