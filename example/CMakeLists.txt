add_executable(example example.c)

target_include_directories(example PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(example imgdata)
