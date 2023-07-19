add_executable(example
    EXCLUDE_FROM_ALL example.c
)

target_include_directories(example
    PRIVATE ${PROJECT_SOURCE_DIR}/${LIB_NAME}/include
)

target_link_libraries(example
    ${LIB_NAME}
)
