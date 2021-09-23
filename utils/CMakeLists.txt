add_library(imgdata SHARED imgdata.c)

target_include_directories(imgdata PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(imgdata PUBLIC png)

MESSAGE("${PROJECT_SRC_DIR}/include")
