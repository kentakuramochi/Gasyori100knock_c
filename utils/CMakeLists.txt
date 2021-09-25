add_library(imgdata EXCLUDE_FROM_ALL SHARED imgdata.c)

target_include_directories(imgdata PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(imgdata PUBLIC png)
