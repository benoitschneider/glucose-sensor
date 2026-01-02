add_library(common_target STATIC
    src/utils.c
)

target_include_directories(common_target PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
