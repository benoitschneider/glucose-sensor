# This file is used by CMake to configure the ARM GCC toolchain.
# It should be passed to CMake using -DCMAKE_TOOLCHAIN_FILE=toolchain-arm-none-eabi.cmake

# Specify the target system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

# Specify the cross compilers
find_program(CMAKE_C_COMPILER arm-none-eabi-gcc)
find_program(CMAKE_CXX_COMPILER arm-none-eabi-g++)
find_program(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy)
find_program(CMAKE_SIZE arm-none-eabi-size)
find_program(CMAKE_AR arm-none-eabi-ar)

if(NOT CMAKE_C_COMPILER)
    message(FATAL_ERROR "arm-none-eabi-gcc not found. Please ensure ARM GCC is in your PATH.")
endif()

# Set default compiler flags for C, CXX, and ASM
# These can be overridden or appended in CMakeLists.txt
set(COMMON_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostdlib")

set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11") # Or c++14/c++17 depending on project needs
set(CMAKE_ASM_FLAGS_INIT "${COMMON_FLAGS}")

# Linker flags
# Specify the path to the nRF52832 linker script.
# This path might need to be adjusted based on your nRF5 SDK installation.
# For this example, we assume it's in a 'linker_scripts' directory at the project root.
# In a real project, you might use a variable to point to the SDK.
set(NRF52_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker_scripts/nrf52_flash.ld")

if(NOT EXISTS "${NRF52_LINKER_SCRIPT}")
    message(FATAL_ERROR "Linker script not found: ${NRF52_LINKER_SCRIPT}. Please ensure it exists or update the path.")
endif()

set(CMAKE_EXE_LINKER_FLAGS_INIT "-T \"${NRF52_LINKER_SCRIPT}\" -Wl,--gc-sections -Wl,--cref -Wl,--defsym=__STACK_SIZE=2048 -Wl,--defsym=__HEAP_SIZE=2048")

# Disable compiler checks for the host system
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# No implicit system includes or libraries
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
