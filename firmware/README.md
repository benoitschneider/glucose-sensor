# Unified Firmware

## Build Instructions

No changes to build instructions, as the build process itself remains the same; only the location of the CI workflow file has changed.

## Dependencies

ARM GCC toolchain (e.g., `gcc-arm-none-eabi-10-2020-q4-major` or similar)
CMake (version 3.10 or newer)
Nordic nRF5 SDK (version 17.0.2 recommended for nRF52832)
nrfjprog (for flashing/debugging)
SEGGER J-Link Software and Documentation Pack (for JLinkGDBServer)
i2c.h
i2c.c
cmake
ninja-build
gcc-arm-none-eabi