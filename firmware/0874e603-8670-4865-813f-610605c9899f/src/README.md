# Develop the ADS1115 specific driver and configuration functions.

## Build Instructions

To compile this code, you will need an ARM GCC toolchain (e.g., `arm-none-eabi-gcc`).

1.  **Replace Mock I2C Driver**: The `ads1115.c` file contains mock I2C functions (`mock_i2c_write`, `mock_i2c_read`, `mock_i2c_write_read`, `mock_delay_ms`). You MUST replace these with your microcontroller's specific I2C driver functions and delay implementation. Uncomment and adapt the `#define` statements in `ads1115.c` to point to your actual driver.
    *   Ensure your I2C driver provides functions with signatures compatible with `I2C_MASTER_WRITE(address, data, len)`, `I2C_MASTER_READ(address, data, len)`, and `I2C_MASTER_WRITE_READ(address, write_data, write_len, read_data, read_len)`. If your I2C driver does not have a combined write-read function, you'll need to implement it using sequential write and read calls.
    *   Implement `DELAY_MS(ms)` using a non-blocking timer or a busy-wait loop appropriate for your RTOS/bare-metal environment.

2.  **Compile**: Assuming your I2C driver is correctly integrated and provides the necessary functions, you can compile the source files:
    ```bash
    arm-none-eabi-gcc -c main.c -o main.o -Wall -Os -I.
    arm-none-eabi-gcc -c ads1115.c -o ads1115.o -Wall -Os -I.
    # Link with your system's startup files and libraries
    arm-none-eabi-gcc main.o ads1115.o -o firmware.elf -T linker_script.ld -nostdlib -Wl,-Map=firmware.map
    ```
    *   `-I.` adds the current directory to the include path for `config.h` and `ads1115.h`.
    *   `-T linker_script.ld` and `-nostdlib` are placeholders; you'll need your specific linker script and standard library configuration for your target microcontroller.
    *   `-Wall -Os` are recommended flags for warnings and optimization.

3.  **Flash**: Flash the generated `firmware.elf` to your target microcontroller using your preferred programmer/debugger.