# Implement the ADS1115 initialization function to configure gain, sampling rate, and channel settings.

## Build Instructions

To compile this code, you will need a cross-compiler for your target ARM microcontroller (e.g., `arm-none-eabi-gcc`).

1.  **Save the files:** Save `main.c`, `ads1115.c`, `ads1115.h`, `config.h`, `i2c_driver.c`, and `i2c_driver.h` into a project directory.
2.  **Compile:** Use a command similar to this (adjusting for your specific MCU and linker script):
    ```bash
    arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -g -O0 -Wall -I. -c main.c -o main.o
    arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -g -O0 -Wall -I. -c ads1115.c -o ads1115.o
    arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -g -O0 -Wall -I. -c i2c_driver.c -o i2c_driver.o
    arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -g -O0 -Wall -T linker_script.ld main.o ads1115.o i2c_driver.o -o firmware.elf
    arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
    ```
    *   Replace `-mcpu=cortex-m4` with your MCU's CPU.
    *   You'll need a `linker_script.ld` appropriate for your target microcontroller.
    *   The mock `i2c_driver.c` uses `printf`, so ensure your toolchain is configured to link with a C library that supports it (e.g., newlib-nano with semihosting or retargeted `_write` for UART output).
3.  **Flash:** The `firmware.bin` can then be flashed to your microcontroller using appropriate tools (e.g., OpenOCD, ST-Link utility, J-Link).

For a production system, the `i2c_driver.c` would be replaced with a hardware-specific implementation using the MCU's HAL or Low-Level drivers.