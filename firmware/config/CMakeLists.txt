add_library(config_target INTERFACE)

target_include_directories(config_target INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
