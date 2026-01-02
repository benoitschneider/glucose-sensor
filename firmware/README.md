# Unified Firmware

## Build Instructions

No specific build instruction updates beyond what CMakeLists.txt changes imply. The new driver files will be automatically included by the updated `drivers/CMakeLists.txt` and linked by `app/src/CMakeLists.txt`.

## Dependencies

ARM GCC toolchain (e.g., `gcc-arm-none-eabi-10-2020-q4-major` or similar)
CMake (version 3.10 or newer)
Nordic nRF5 SDK (version 17.0.2 recommended for nRF52832)
nrfjprog (for flashing/debugging)
SEGGER J-Link Software and Documentation Pack (for JLinkGDBServer)
i2c.h
i2c.c