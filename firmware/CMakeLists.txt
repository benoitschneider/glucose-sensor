CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(GlucoseSensorFirmware C ASM)

# Set the build type (e.g., Debug, Release)
set(CMAKE_BUILD_TYPE Debug)

# Toolchain file for ARM GCC
# This path should be adjusted based on where your toolchain file is located.
# For example, it could be in a 'toolchain' directory at the project root.
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchain.cmake" CACHE FILEPATH "Path to the ARM GCC toolchain file")

# Define the target system and processor for cross-compilation
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Include subdirectories
add_subdirectory(app)
# add_subdirectory(drivers)
# add_subdirectory(ble)
# add_subdirectory(common)
# add_subdirectory(config)
