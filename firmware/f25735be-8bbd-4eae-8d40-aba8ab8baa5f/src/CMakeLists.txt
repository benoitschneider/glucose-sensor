# Root CMakeLists.txt for Glucose Sensor Firmware
# Defines the overall project, toolchain, and includes subdirectories.

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project name and languages
project(GlucoseSensorFirmware C ASM)

# --- Configuration Variables ---

# Set the target architecture and MCU
set(TARGET_MCU nrf52832)

# Path to the Nordic nRF5 SDK (adjust as needed)
# It's recommended to place the SDK in a known location relative to the project
# or set this as an environment variable.
set(NRF5_SDK_PATH "${CMAKE_SOURCE_DIR}/nRF5_SDK_17.0.2_offline_doc") # Example path

if(NOT EXISTS "${NRF5_SDK_PATH}")
    message(FATAL_ERROR "Nordic nRF5 SDK not found at: ${NRF5_SDK_PATH}")
endif()

# Toolchain file for ARM GCC
# This file typically defines compiler flags, linker scripts, and target architecture.
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/toolchain-arm-none-eabi.cmake")

# Set build type (Debug/Release) - default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Output directories for build artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# --- Global Include Directories ---
# These are common includes for the entire project or SDK-related
include_directories(
    ${NRF5_SDK_PATH}/components/toolchain/cmsis/include
    ${NRF5_SDK_PATH}/components/device
    ${NRF5_SDK_PATH}/components/libraries/util
    ${NRF5_SDK_PATH}/components/boards
    ${NRF5_SDK_PATH}/components/drivers_nrf/nrf_soc_src
    ${NRF5_SDK_PATH}/components/softdevice/s132/headers
    ${NRF5_SDK_PATH}/components/softdevice/common
    ${NRF5_SDK_PATH}/components/softdevice/s132/headers/nrf52
    ${CMAKE_SOURCE_DIR}/config # For sdk_config.h
)

# --- Add Subdirectories ---
# Each subdirectory should have its own CMakeLists.txt to define its sources and targets.
add_subdirectory(app)
add_subdirectory(drivers)
add_subdirectory(ble)
add_subdirectory(common)
add_subdirectory(config)

# --- Custom Targets for Flashing/Debugging ---
# These targets depend on the main firmware binary being built.

# Flashing target using nrfjprog
# Ensure nrfjprog is in your system's PATH or provide its full path.
find_program(NRFJPROG_EXECUTABLE nrfjprog)

if(NRFJPROG_EXECUTABLE)
    add_custom_target(flash
        COMMAND ${NRFJPROG_EXECUTABLE} --eraseall -f nrf52
        COMMAND ${NRFJPROG_EXECUTABLE} --program ${CMAKE_BINARY_DIR}/bin/GlucoseSensorFirmware.hex -f nrf52 --chiperase --verify
        COMMAND ${NRFJPROG_EXECUTABLE} --reset -f nrf52
        DEPENDS GlucoseSensorFirmware
        COMMENT "Flashing GlucoseSensorFirmware.hex to nRF52832"
    )

    add_custom_target(flash_softdevice
        COMMAND ${NRFJPROG_EXECUTABLE} --program ${NRF5_SDK_PATH}/components/softdevice/s132/hex/s132_nrf52_6.1.1_softdevice.hex -f nrf52 --chiperase --verify
        COMMAND ${NRFJPROG_EXECUTABLE} --reset -f nrf52
        COMMENT "Flashing S132 SoftDevice to nRF52832"
    )

    add_custom_target(erase_chip
        COMMAND ${NRFJPROG_EXECUTABLE} --eraseall -f nrf52
        COMMENT "Erasing entire nRF52832 chip"
    )
else()
    message(WARNING "nrfjprog not found. Flash targets will not be available.")
endif()

# Debugging target (example using J-Link GDB server)
# This is a placeholder and might require more sophisticated setup depending on IDE/debugger.
find_program(JLINKGDB_EXECUTABLE JLinkGDBServer)

if(JLINKGDB_EXECUTABLE)
    add_custom_target(debug_server
        COMMAND ${JLINKGDB_EXECUTABLE} -device ${TARGET_MCU} -if SWD -speed 4000 -port 2331
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMENT "Starting J-Link GDB Server for ${TARGET_MCU}"
    )
else()
    message(WARNING "JLinkGDBServer not found. Debug server target will not be available.")
endif()

# Final message
message(STATUS "Project configured for ${TARGET_MCU} with NRF5 SDK at ${NRF5_SDK_PATH}")
