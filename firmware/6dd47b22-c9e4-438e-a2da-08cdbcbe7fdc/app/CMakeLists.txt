cmake_minimum_required(VERSION 3.16)

# Define the executable name
set(TARGET_NAME GlucoseSensorFirmware)

# Add the executable target
add_executable(${TARGET_NAME}
    src/main.c
)

# Specify include directories for the application
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    # Add other necessary include paths here, e.g., for HAL, CMSIS
    # ${CMAKE_SOURCE_DIR}/drivers/inc
    # ${CMAKE_SOURCE_DIR}/hal/inc
)

# Link common libraries/toolchain settings from the root CMakeLists.txt
# This assumes the root CMakeLists.txt handles linking of startup files, linker scripts, etc.
# For a minimal build, we might not need explicit library linking here if it's handled globally.
# If using a specific linker script, it would be passed via target_link_options or similar.

# Set output directories for the build artifacts
# These variables are typically set in the root CMakeLists.txt or toolchain file
# but can be overridden here if needed for specific targets.
# For this task, we assume they are set globally to point to the build directory.

# Add a custom command to generate the .hex file from the .elf file
# This command will run after the executable is linked.
add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET_NAME}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}.hex
    COMMENT "Generating ${TARGET_NAME}.hex"
    VERBATIM
)

# Optionally, add a custom command to generate a .bin file
add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_NAME}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}.bin
    COMMENT "Generating ${TARGET_NAME}.bin"
    VERBATIM
)
