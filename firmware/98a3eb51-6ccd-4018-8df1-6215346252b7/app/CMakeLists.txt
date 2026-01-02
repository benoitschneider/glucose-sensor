cmake_minimum_required(VERSION 3.10)

# Define the application target
project(GlucoseSensorApp C)

# Source files for the application
set(APP_SOURCES
    src/main.c
    # Add other application source files here as they are created
)

# Include directories for the application
# Add project-wide includes here, or specific ones for the app
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    # Add common include directories like config, drivers etc.
    # ${CMAKE_SOURCE_DIR}/config
)

# Create the executable target
add_executable(${PROJECT_NAME} ${APP_SOURCES})

# Link libraries (e.g., drivers, HAL, BLE stack)
# target_link_libraries(${PROJECT_NAME} 
#     # driver_library
#     # ble_stack_library
# )

# Post-build steps (e.g., converting .elf to .hex or .bin)
# This example uses objcopy to create a .hex file
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
    BYPRODUCTS ${PROJECT_NAME}.hex ${PROJECT_NAME}.bin
    COMMENT "Generating .hex and .bin files for ${PROJECT_NAME}"
)
