file(GLOB FILES "./*.c")

foreach(FILE ${FILES})
    get_filename_component(TARGET ${FILE} NAME_WE)
    add_executable(${TARGET} ${FILE})
    target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${TARGET} imgdata)
endforeach()
