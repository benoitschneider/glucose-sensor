cmake_minimum_required(VERSION 3.10)

# Define the project name and supported languages
project(GlucoseSensorFirmware C ASM)

# --- Toolchain Configuration ---
# This assumes a custom toolchain file will be provided
# For example, toolchain-arm-none-eabi.cmake
# CMAKE_TOOLCHAIN_FILE should be set when configuring CMake, e.g.:
# cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/toolchain-arm-none-eabi.cmake ..
# If not set externally, provide a default or error.
if(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE is not set. Please specify your ARM GCC toolchain file.")
endif()

# Set target system and processor for cross-compilation
# These are typically set by the toolchain file, but can be overridden here if necessary.
# CMAKE_SYSTEM_NAME is often 'Generic' for bare-metal, or 'Unix'/'Linux' if using an OS.
# CMAKE_SYSTEM_PROCESSOR specifies the architecture.
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Set the build type. Can be overridden by -DCMAKE_BUILD_TYPE=Release during configuration.
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release MinSizeRel RelWithDebInfo" FORCE)

# --- Compiler Definitions (often set by toolchain file) ---
# Ensure the correct ARM GCC compilers are used.
# These variables are typically set within the CMAKE_TOOLCHAIN_FILE.
# If you need to explicitly set them here, uncomment and modify:
# set(CMAKE_C_COMPILER arm-none-eabi-gcc)
# set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
# set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# --- Global Project Variables ---
# Define common include directories, source groups, etc.
# This can be expanded as the project grows.

# --- Subdirectory Inclusion ---
# Include subdirectories where application code, drivers, etc., reside.
# The 'app' directory contains the main application logic.
add_subdirectory(app)

# Placeholder for other subdirectories as they are created
# add_subdirectory(drivers)
# add_subdirectory(ble)
# add_subdirectory(common)
# add_subdirectory(config)

# --- Output Directories ---
# Set default output directories for binaries and libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Compiler Flags (Common) ---
# These flags are common for embedded ARM development.
# They can be refined in specific CMakeLists.txt files for targets.
set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wall -Wextra -Wno-unused-parameter -fno-strict-aliasing -fno-builtin -ffunction-sections -fdata-sections -fno-common -std=c11")
set(CMAKE_ASM_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T \"${CMAKE_SOURCE_DIR}/linkerscript.ld\" -Wl,--gc-sections -Wl,--cref -Wl,--no-warn-rwx-segments")

# Debug specific flags
set(CMAKE_C_FLAGS_DEBUG_INIT "${CMAKE_C_FLAGS_INIT} -Og -g -DDEBUG")
set(CMAKE_ASM_FLAGS_DEBUG_INIT "${CMAKE_ASM_FLAGS_INIT} -g")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT}")

# Release specific flags
set(CMAKE_C_FLAGS_RELEASE_INIT "${CMAKE_C_FLAGS_INIT} -Os -DNDEBUG")
set(CMAKE_ASM_FLAGS_RELEASE_INIT "${CMAKE_ASM_FLAGS_INIT}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT}")

# MinSizeRel specific flags
set(CMAKE_C_FLAGS_MINSIZEREL_INIT "${CMAKE_C_FLAGS_INIT} -Os -DNDEBUG")
set(CMAKE_ASM_FLAGS_MINSIZEREL_INIT "${CMAKE_ASM_FLAGS_INIT}")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT}")

# RelWithDebInfo specific flags
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "${CMAKE_C_FLAGS_INIT} -Og -g -DNDEBUG")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO_INIT "${CMAKE_ASM_FLAGS_INIT} -g")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT}")
