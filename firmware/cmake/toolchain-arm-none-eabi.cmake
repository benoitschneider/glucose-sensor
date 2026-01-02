# CMake Toolchain file for ARM GCC (arm-none-eabi)
# This file is used by CMake to configure the cross-compilation environment.

# Specify the target system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross compiler
# You might need to adjust this path based on your ARM GCC installation.
find_program(TOOLCHAIN_PREFIX arm-none-eabi-gcc)
if(NOT TOOLCHAIN_PREFIX)
    message(FATAL_ERROR "ARM GCC (arm-none-eabi-gcc) not found. Please ensure it's in your PATH.")
endif()

set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Set common compiler flags for ARM Cortex-M4F (nRF52832)
set(COMMON_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")

# C Compiler Flags
set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=c99 -Wall -Werror -g3 -DNRF52 -DNRF52832_XXAA -DSOFTDEVICE_PRESENT")
set(CMAKE_C_FLAGS_DEBUG_INIT "-O0 -gdwarf-2")
set(CMAKE_C_FLAGS_RELEASE_INIT "-Os -DNDEBUG")

# C++ Compiler Flags (if C++ is used)
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11 -fno-exceptions -fno-rtti -Wall -Werror -g3 -DNRF52 -DNRF52832_XXAA -DSOFTDEVICE_PRESENT")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-O0 -gdwarf-2")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Os -DNDEBUG")

# Assembler Flags
set(CMAKE_ASM_FLAGS_INIT "${COMMON_FLAGS} -x assembler-with-cpp")

# Linker Flags
# Linker script for nRF52832 with S132 SoftDevice (adjust path if needed)
set(LINKER_SCRIPT "${NRF5_SDK_PATH}/components/toolchain/gcc/nrf52832_xxaa_s132.ld")

if(NOT EXISTS "${LINKER_SCRIPT}")
    message(FATAL_ERROR "Linker script not found at: ${LINKER_SCRIPT}")
endif()

set(CMAKE_EXE_LINKER_FLAGS_INIT "-L${NRF5_SDK_PATH}/components/toolchain/gcc -T ${LINKER_SCRIPT} -Wl,--gc-sections -Wl,--sort-common -Wl,--cref -Wl,--check-sections -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--defsym=__stack_size__=2048 -Wl,--defsym=__heap_size__=2048 -nostartfiles -nodefaultlibs -lc -lnosys -lm -lgcc")

# Do not search for programs in the build host's path.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# For libraries and headers in the target environment, search in the specified root path.
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set the sysroot for the toolchain
# This is often the path to the ARM GCC installation itself.
# If not set, CMake might try to use host system libraries.
# set(CMAKE_SYSROOT "/path/to/arm-none-eabi-gcc/install") # Uncomment and set if needed

# Add the path to the toolchain binaries to CMAKE_PROGRAM_PATH
# This helps CMake find utilities like objcopy, size, etc.
# get_filename_component(ARM_GCC_BIN_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
# list(APPEND CMAKE_PROGRAM_PATH "${ARM_GCC_BIN_DIR}")

# Add post-build commands to generate .hex and .bin files
add_custom_command(
    TARGET GlucoseSensorFirmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:GlucoseSensorFirmware> ${CMAKE_BINARY_DIR}/bin/GlucoseSensorFirmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:GlucoseSensorFirmware> ${CMAKE_BINARY_DIR}/bin/GlucoseSensorFirmware.bin
    COMMENT "Generating .hex and .bin files"
)
