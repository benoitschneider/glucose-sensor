# Develop the ADS1115 specific driver and configuration functions.

## Build Instructions

To compile this code, you will need an ARM GCC toolchain. Assuming you have `arm-none-eabi-gcc` installed:

1.  **Create an object file for each .c file:**
    `arm-none-eabi-gcc -c main.c -o main.o -Wall -O2 -I. -mcpu=cortex-m4 -mthumb`
    `arm-none-eabi-gcc -c i2c_driver.c -o i2c_driver.o -Wall -O2 -I. -mcpu=cortex-m4 -mthumb`
    `arm-none-eabi-gcc -c ads1115_driver.c -o ads1115_driver.o -Wall -O2 -I. -mcpu=cortex-m4 -mthumb`

    *Note: `-mcpu=cortex-m4 -mthumb` are examples for a Cortex-M4 microcontroller. Adjust based on your target MCU.* `-I.` tells the compiler to look for headers in the current directory.

2.  **Link the object files to create an executable (e.g., .elf file):**
    `arm-none-eabi-gcc main.o i2c_driver.o ads1115_driver.o -o firmware.elf -nostartfiles -Tlinker_script.ld`

    *Note: You will need a `linker_script.ld` specific to your microcontroller to define memory regions.* `-nostartfiles` is often used in embedded systems to avoid standard library startup code.

3.  **Convert the .elf file to a binary or hex for flashing:**
    `arm-none-eabi-objcopy -O binary firmware.elf firmware.bin`
    `arm-none-eabi-objcopy -O ihex firmware.elf firmware.hex`

**Important:** The `i2c_driver.c` contains placeholder functions. For a real embedded system, you must replace these with the actual I2C HAL (Hardware Abstraction Layer) functions provided by your microcontroller vendor (e.g., STM32Cube HAL, nRF Connect SDK, etc.). The `ads1115_delay_ms` and `main_delay_ms` functions also need to be replaced with non-blocking, timer-based delays or RTOS-aware delays for a production system.