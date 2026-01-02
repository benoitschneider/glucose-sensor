# Define the root CMakeLists.txt and initial project structure.

## Build Instructions

To build the project:

1.  **Install ARM GCC Toolchain**: Ensure you have `arm-none-eabi-gcc` installed and accessible in your system's PATH. You can download it from ARM's website (e.g., GNU Arm Embedded Toolchain).
2.  **Create Project Directory Structure**: Manually create the following directories:
    ```
    . # Project Root
    ├── CMakeLists.txt
    ├── app
    │   ├── CMakeLists.txt
    │   ├── inc
    │   └── src
    │       └── main.c
    ├── drivers
    ├── ble
    ├── common
    ├── config
    ├── build
    ├── linkerscript.ld
    └── toolchain-arm-none-eabi.cmake
    ```
    Place the generated `CMakeLists.txt` in the root, `app/CMakeLists.txt` in `app/`, `main.c` in `app/src/`, `app_config.h` in `app/inc/`, `linkerscript.ld` in the root, and `toolchain-arm-none-eabi.cmake` in the root.
3.  **Configure CMake**: Navigate to the `build` directory and run CMake, specifying the toolchain file:
    ```bash
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm-none-eabi.cmake ..
    ```
    *   You can specify the build type (e.g., Release) by adding `-DCMAKE_BUILD_TYPE=Release`.
    *   If your `arm-none-eabi-gcc` is not in your system PATH, you might need to modify `toolchain-arm-none-eabi.cmake` to point to the full path of the compiler executables.
4.  **Build the Project**: After successful configuration, build the project:
    ```bash
    cmake --build .
    ```
    This will compile `main.c` and link it, producing `GlucoseSensorApp.elf`, `GlucoseSensorApp.hex`, and `GlucoseSensorApp.bin` in the `build/bin` directory.

**Note**: The `linkerscript.ld` provided is a generic example for nRF52832. For a real project, it would typically be provided by the MCU vendor's SDK (e.g., Nordic's nRF5 SDK) and might need to be adjusted for SoftDevice usage (if BLE is involved) to reserve memory regions.
