cmake_minimum_required(VERSION 3.16)

project(GlucoseSensorFirmware C CXX ASM)

# Set the build type if not already set (e.g., by command line -DCMAKE_BUILD_TYPE=Debug)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Include the custom toolchain file for ARM GCC
# This file is expected to be in the project root or specified via CMAKE_TOOLCHAIN_FILE
# For local development, it's often passed via `cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-arm-none-eabi.cmake ..`
# However, we can also try to include it directly if it's placed in the project root.
# For this task, we'll assume it's passed via command line or a wrapper script.

# Define common compiler flags
add_compile_options(
    -Wall
    -Wextra
    -Wno-unused-parameter # Often useful for embedded, can be removed later
    -fno-builtin
    -fno-strict-aliasing
    -fshort-enums
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -fno-rtti
)

# Add include directories
include_directories(
    include
    src
)

# Add subdirectories for modules (e.g., drivers, app)
add_subdirectory(src)

# Define the main firmware executable target
add_executable(${PROJECT_NAME}.elf src/main.c)

# Link the linker script (this will be handled by the toolchain file or direct linker flags)
# For demonstration, we'll add a placeholder for the linker script path.
# In a real scenario, this path would be resolved by the toolchain file or a variable.
# We'll assume the toolchain file or CMAKE_EXE_LINKER_FLAGS handles this.
# For now, let's add a dummy linker flag that will be overridden or complemented.

# Post-build steps for generating .hex and .bin files
add_custom_command(
    TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMENT "Generating .hex and .bin files"
)

# Optional: Create a 'flash' target (requires external tools like nrfjprog)
# add_custom_target(flash ALL
#     COMMAND nrfjprog --program ${PROJECT_NAME}.hex --chiperase --reset
#     DEPENDS ${PROJECT_NAME}.elf
#     COMMENT "Flashing firmware to device"
# )
