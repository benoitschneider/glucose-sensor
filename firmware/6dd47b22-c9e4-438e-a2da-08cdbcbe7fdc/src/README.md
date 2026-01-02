# Configure the 'app' subdirectory and achieve a successful 'hello world' build.

## Build Instructions

Assuming a `build` directory exists at the project root and CMake is configured with a toolchain file for `arm-none-eabi`:

1.  **Navigate to the build directory:**
    `cd build`

2.  **Run CMake to configure the project (if not already done):**
    `cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm-none-eabi.cmake`
    (Replace `../toolchain-arm-none-eabi.cmake` with the actual path to your toolchain file)

3.  **Build the project:**
    `cmake --build .`
    or if using Makefiles:
    `make`

This will compile `app/src/main.c`, link it into `GlucoseSensorFirmware.elf`, and then generate `GlucoseSensorFirmware.hex` and `GlucoseSensorFirmware.bin` in the `build` directory.