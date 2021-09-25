file(GLOB files "./*.c")

foreach(file ${files})
    get_filename_component(target ${file} NAME_WE)
    add_executable(${target} ${file})
    target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${target} imgdata)
endforeach()
