# CMake toolchain file for ARM GCC (arm-none-eabi)
# This file should be specified with -DCMAKE_TOOLCHAIN_FILE=path/to/this/file.cmake

# Specify the target system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross compilers
# Adjust the path to your ARM GCC installation if necessary
# A common installation path might be /usr/local/gcc-arm-none-eabi-xxxx.xx/bin
set(TOOLCHAIN_PREFIX arm-none-eabi-)

find_program(CMAKE_C_COMPILER NAMES ${TOOLCHAIN_PREFIX}gcc)
find_program(CMAKE_CXX_COMPILER NAMES ${TOOLCHAIN_PREFIX}g++)
find_program(CMAKE_ASM_COMPILER NAMES ${TOOLCHAIN_PREFIX}gcc)

# Check if compilers were found
if(NOT CMAKE_C_COMPILER OR NOT CMAKE_CXX_COMPILER OR NOT CMAKE_ASM_COMPILER)
    message(FATAL_ERROR "ARM GCC compilers not found. Please ensure '${TOOLCHAIN_PREFIX}gcc' and '${TOOLCHAIN_PREFIX}g++' are in your PATH or specify their full path.")
endif()

# Specify the archiver and object copy tools
find_program(CMAKE_AR NAMES ${TOOLCHAIN_PREFIX}ar)
find_program(CMAKE_OBJCOPY NAMES ${TOOLCHAIN_PREFIX}objcopy)
find_program(CMAKE_SIZE NAMES ${TOOLCHAIN_PREFIX}size)
find_program(CMAKE_OBJDUMP NAMES ${TOOLCHAIN_PREFIX}objdump)

# Set flags for finding libraries and headers
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set common compiler flags for the nRF52832 (Cortex-M4F)
# These are base flags; specific project flags can be added in CMakeLists.txt
set(COMMON_MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")

set(CMAKE_C_FLAGS_INIT "${COMMON_MCU_FLAGS} -Wall -Wextra -Wno-unused-parameter -fno-strict-aliasing -fno-builtin -ffunction-sections -fdata-sections -fno-common -std=c11" CACHE STRING "C Compiler flags")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_MCU_FLAGS} -Wall -Wextra -Wno-unused-parameter -fno-strict-aliasing -fno-builtin -ffunction-sections -fdata-sections -fno-common -fno-rtti -fno-exceptions -std=c++11" CACHE STRING "C++ Compiler flags")
set(CMAKE_ASM_FLAGS_INIT "${COMMON_MCU_FLAGS}" CACHE STRING "ASM Compiler flags")

# Linker flags
# -T specifies the linker script
# --gc-sections enables garbage collection of unused sections
# --cref generates a cross-reference table
set(CMAKE_EXE_LINKER_FLAGS_INIT "${COMMON_MCU_FLAGS} -Wl,--gc-sections -Wl,--cref -Wl,--no-warn-rwx-segments" CACHE STRING "Linker flags")

# Set the sysroot if available (e.g., for newlib-nano)
# set(CMAKE_SYSROOT "/path/to/arm-none-eabi/sysroot")

# Ensure the linker script is used
# This is typically set in the main CMakeLists.txt or target-specific CMakeLists.txt
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T \"${CMAKE_SOURCE_DIR}/linkerscript.ld\"")
