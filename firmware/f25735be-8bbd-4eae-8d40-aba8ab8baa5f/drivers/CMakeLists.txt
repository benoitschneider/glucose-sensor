# drivers/CMakeLists.txt
# This file can be used to define a library for all drivers or include individual driver subdirectories.

# Example: Add a subdirectory for a specific sensor driver
# add_subdirectory(sensor_driver)

# For now, we'll define a placeholder library if no specific drivers are added yet.
# This ensures the build system can still link to 'drivers_lib' if referenced elsewhere.
add_library(drivers_lib INTERFACE)

# Add common include directories for all drivers
target_include_directories(drivers_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Example of how to add a source file directly if no sub-directory is needed for a simple driver
# set(DRIVER_SOURCES
#     src/some_driver.c
# )
# target_sources(drivers_lib PRIVATE ${DRIVER_SOURCES})
