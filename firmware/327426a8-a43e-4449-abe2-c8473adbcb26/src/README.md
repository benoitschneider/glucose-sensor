# Implement a configurable moving average filter for glucose readings.

## Build Instructions

To compile this code, you would typically use an ARM GCC toolchain (e.g., `arm-none-eabi-gcc`).

1.  **Save the files**: Save `main.c`, `glucose_filter.c`, `glucose_filter.h`, `nvm_storage.h`, and `config.h` into a single directory.

2.  **Compile**: Use a command similar to this (adjusting for your specific compiler and target architecture):
    ```bash
    arm-none-eabi-gcc -Wall -Wextra -std=c99 -Os -c main.c -o main.o
    arm-none-eabi-gcc -Wall -Wextra -std=c99 -Os -c glucose_filter.c -o glucose_filter.o
    arm-none-eabi-gcc -o glucose_filter_app main.o glucose_filter.o
    ```
    For a hosted environment (like a PC for testing), you can use `gcc`:
    ```bash
    gcc -Wall -Wextra -std=c99 -Os -c main.c -o main.o
    gcc -Wall -Wextra -std=c99 -Os -c glucose_filter.c -o glucose_filter.o
    gcc -o glucose_filter_app main.o glucose_filter.o
    ./glucose_filter_app
    ```

3.  **Link**: Link the object files to create an executable. For embedded systems, this would involve a linker script and potentially linking against a HAL/SPL library.

This example includes a mock NVM implementation within `glucose_filter.c` for self-containment and demonstration. In a real project, the `nvm_storage.c` (if it existed) would contain the actual flash/EEPROM access routines.