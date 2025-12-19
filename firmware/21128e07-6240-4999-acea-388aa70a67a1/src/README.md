# Implement a configurable moving average filter for glucose readings.

## Build Instructions

To compile this code, you would typically use an ARM GCC toolchain (e.g., `arm-none-eabi-gcc`).

1.  **Save the files:** Save `main.c`, `glucose_filter.c`, `glucose_filter.h`, and `config.h` into a project directory.
2.  **Compile:**
    ```bash
    arm-none-eabi-gcc -c main.c -o main.o -I.
    arm-none-eabi-gcc -c glucose_filter.c -o glucose_filter.o -I.
    ```
3.  **Link:** (This example doesn't include a linker script or startup code for a specific MCU, so a full link might not produce a runnable binary without further setup.)
    ```bash
    arm-none-eabi-gcc main.o glucose_filter.o -o firmware.elf -nostdlib -Wl,-T,linker_script.ld
    ```
    *Replace `linker_script.ld` with your target microcontroller's linker script.*

**For basic testing on a host PC (without ARM target):**

1.  You can compile and run `main.c` directly using a standard GCC compiler. Note that `rand()` is used, so you might want to include `<stdlib.h>` and `<time.h>` and call `srand(time(NULL))` in `main` for better randomness.
2.  Compile:
    ```bash
    gcc main.c glucose_filter.c -o glucose_filter_test -I.
    ```
3.  Run:
    ```bash
    ./glucose_filter_test
    ```
    *Note: The NVM module is mocked, so it will behave like a RAM-based storage on a host PC.*