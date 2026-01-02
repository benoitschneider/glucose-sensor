# toolchain.cmake
# This file defines the toolchain for cross-compilation.
# It is referenced by the top-level CMakeLists.txt.

# This file is crucial for telling CMake how to find the ARM compiler.

# Specify the target system
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the compilers
# In GitHub Actions, `gcc-arm-none-eabi` is installed and its binaries are in PATH.
# So, CMake can often find them directly. However, explicitly setting them is safer.
SET(TOOLCHAIN_PREFIX arm-none-eabi-)
SET(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
SET(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)

# Specify utilities
SET(CMAKE_OBJCOPY      ${TOOLCHAIN_PREFIX}objcopy)
SET(CMAKE_SIZE         ${TOOLCHAIN_PREFIX}size)
SET(CMAKE_AR           ${TOOLCHAIN_PREFIX}ar)

# Search for programs in the build host's PATH
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target environment
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Define the target architecture flags (can also be in CMakeLists.txt)
# SET(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
# SET(CMAKE_CXX_FLAGS_INIT "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")

# Add any specific sysroot or include paths if your SDK is not globally installed
# For example, if you have a local nRF SDK:
# SET(NRF_SDK_PATH "${CMAKE_SOURCE_DIR}/nrf_sdk")
# SET(CMAKE_FIND_ROOT_PATH "${NRF_SDK_PATH}")
# INCLUDE_DIRECTORIES("${NRF_SDK_PATH}/components/toolchain/cmsis/include")
