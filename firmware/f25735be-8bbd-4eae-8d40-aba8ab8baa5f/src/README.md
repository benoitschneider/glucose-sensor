# Set up a basic CMake-based build system for the firmware project, including the main application, necessary drivers, and a structure for future expansion.

## Build Instructions

To build the project:

1.  **Prerequisites:**
    *   Install ARM GCC toolchain (e.g., `arm-none-eabi-gcc`). Ensure it's in your system's PATH.
    *   Install CMake (version 3.10 or newer).
    *   Download Nordic nRF5 SDK (version 17.0.2 recommended) and place it in the project root directory, or adjust the `NRF5_SDK_PATH` variable in `CMakeLists.txt`.
    *   Install `nrfjprog` for flashing (part of nRF Command Line Tools).

2.  **Directory Structure:**
    Ensure your project structure matches the one specified:
    ```
    ├── CMakeLists.txt
    ├── app
    │   └── src
    │       └── main.c
    │   └── CMakeLists.txt
    ├── drivers
    │   └── CMakeLists.txt
    ├── ble
    │   └── CMakeLists.txt
    ├── common
    │   └── utils.c
    │   └── utils.h
    │   └── CMakeLists.txt
    ├── config
    │   └── sdk_config.h
    │   └── CMakeLists.txt
    ├── cmake
    │   └── toolchain-arm-none-eabi.cmake
    └── nRF5_SDK_17.0.2_offline_doc (or your SDK path)
    ```

3.  **Generate Build Files:**
    Navigate to the project root directory in your terminal and run:
    ```bash
    mkdir build
    cd build
    cmake ..
    ```
    This will generate platform-specific build files (e.g., Makefiles on Linux/macOS, Visual Studio projects on Windows).

4.  **Compile the Firmware:**
    From the `build` directory, execute the build command:
    ```bash
    cmake --build .
    # Or, if using Makefiles:
    # make
    ```
    Upon successful compilation, you will find `GlucoseSensorFirmware.elf`, `GlucoseSensorFirmware.hex`, and `GlucoseSensorFirmware.bin` in the `build/bin` directory.

5.  **Flash the Firmware (Optional):**
    From the `build` directory, you can use the custom CMake targets:
    *   To flash the SoftDevice (if not already present or needs update):
        ```bash
        cmake --build . --target flash_softdevice
        # Or:
        # make flash_softdevice
        ```
    *   To flash the application firmware:
        ```bash
        cmake --build . --target flash
        # Or:
        # make flash
        ```
    *   To erase the entire chip:
        ```bash
        cmake --build . --target erase_chip
        # Or:
        # make erase_chip
        ```

6.  **Start Debug Server (Optional):**
    From the `build` directory:
    ```bash
    cmake --build . --target debug_server
    # Or:
    # make debug_server
    ```
    This will start a J-Link GDB server, which you can then connect to with a GDB client or IDE.