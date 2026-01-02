add_library(drivers_target STATIC
    src/ads1115.c
    src/i2c.c
)

target_include_directories(drivers_target PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)