# common/CMakeLists.txt
# Defines a library for common utility functions.

set(COMMON_SOURCES
    utils.c
)

add_library(common_lib STATIC ${COMMON_SOURCES})

# Add include directories for common utilities
target_include_directories(common_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
