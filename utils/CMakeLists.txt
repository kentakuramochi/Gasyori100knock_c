add_library(imgdata STATIC imgdata.c)

target_include_directories(imgdata PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(imgdata PUBLIC png)
