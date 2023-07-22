file(GLOB FILES "./*.c")

foreach(FILE ${FILES})
    get_filename_component(TARGET ${FILE} NAME_WE)
    add_executable(${TARGET} ${FILE})
    target_link_libraries(${TARGET} ${LIB_NAME} m)
endforeach()
