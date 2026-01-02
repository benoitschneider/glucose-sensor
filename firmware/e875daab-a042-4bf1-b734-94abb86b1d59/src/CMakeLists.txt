# CMakeLists.txt
# Top-level CMake file for the firmware project.
# This is a basic example; a real project would be more complex.

cmake_minimum_required(VERSION 3.15)

project(FirmwareProject C ASM)

# Set the target architecture and toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Path to the GCC ARM Embedded toolchain (expected by the CI workflow)
# For local development, you might set this via an environment variable or a specific path.
# In CI, 'gcc-arm-none-eabi' is installed and found in PATH.
# set(TOOLCHAIN_PREFIX arm-none-eabi-)

# Include the toolchain file (this will be provided by the parent requirement)
# For CI, this file needs to exist at the specified path.
# A placeholder `toolchain.cmake` is assumed to be at the root for this example.
# You would typically define CMAKE_C_COMPILER, CMAKE_CXX_COMPILER, etc., within it.

# Define output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler flags for nRF52832
# These are general flags; specific SDKs would add more.
set(COMMON_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wall -Wextra")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=c99" CACHE STRING "C Compiler flags")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++11" CACHE STRING "C++ Compiler flags")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}" CACHE STRING "ASM Compiler flags")

# Linker flags (placeholder - actual flags depend on linker script, memory map, etc.)
# A real project would include a linker script via -T linker_script.ld
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -Wl,--sort-common -Wl,--sort-section=alignment -nostartfiles -nodefaultlibs" CACHE STRING "Linker flags")

# Include directories
include_directories(
    .
    ${CMAKE_SOURCE_DIR}/drivers
    # Add other include paths like SDK headers here
)

# Source files
set(PROJECT_SOURCES
    main.c
    drivers/gpio.c
    drivers/uart.c
    # Add other source files here
)

# Create an executable target
add_executable(${PROJECT_NAME}.elf ${PROJECT_SOURCES})

# Add post-build steps to generate .hex and .bin files (optional for CI, but good practice)
add_custom_command(
    TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Generating .hex and .bin files"
)

# Example of how you might include a subdirectory for more complex projects
# add_subdirectory(app)
# add_subdirectory(drivers)

# Ensure the toolchain file is present for local development or CI
# This is a placeholder for the actual toolchain file that would define
# CMAKE_C_COMPILER, CMAKE_CXX_COMPILER, etc.
# For CI, `gcc-arm-none-eabi` is installed, so CMake might find it directly,
# but explicitly defining compilers in `toolchain.cmake` is more robust.
# A minimal toolchain.cmake might look like:
# SET(CMAKE_C_COMPILER   arm-none-eabi-gcc)
# SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)
# SET(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
# SET(CMAKE_OBJCOPY      arm-none-eabi-objcopy)
# SET(CMAKE_SIZE         arm-none-eabi-size)
# SET(CMAKE_AR           arm-none-eabi-ar)
