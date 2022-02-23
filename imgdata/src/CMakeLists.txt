add_library(${LIB_NAME} EXCLUDE_FROM_ALL SHARED
    imgdata.c
)

# omit warning options for a situation that `argc` is not used
#target_compile_options(${LIB_NAME}
#    PUBLIC -Wall -Wextra -Wpedantic -Werror
#)

target_include_directories(${LIB_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/${LIB_NAME}/include
)

target_link_libraries(${LIB_NAME} PUBLIC
    png
)
