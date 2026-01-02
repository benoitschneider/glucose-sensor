# This is a placeholder toolchain file for ARM GCC.
# You will need to adjust the paths and prefixes based on your specific ARM GCC installation.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross compiler
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

# Specify target architecture and floating point unit
set(MCU_FAMILY nRF52832)
set(ARM_ARCH cortex-m4)
set(ARM_FLOAT_ABI hard)

# Common compiler flags
set(COMMON_FLAGS "-mcpu=${ARM_ARCH} -mthumb -mfloat-abi=${ARM_FLOAT_ABI} -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=c11" CACHE STRING "C Compiler Flags")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++11" CACHE STRING "C++ Compiler Flags")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}" CACHE STRING "ASM Compiler Flags")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -Wl,--print-memory-usage" CACHE STRING "Linker Flags")

# Search for programs in the toolchain path
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
