# Implement a 1Hz sampling mechanism and data buffering for the electrochemical sensor.

## Build Instructions

To compile this code, you will need an ARM GCC toolchain (e.g., `arm-none-eabi-gcc`).

1.  **Save the files**: Save `main.c`, `sensor_sampling.c`, `sensor_sampling.h`, `timer_driver.h`, `ads1115_driver.h`, and `config.h` in a common directory.
2.  **Compile**: Use a command similar to the following. Note that this is a basic compilation command and a real project would use a Makefile or build system.

    ```bash
    arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -g -O0 -Wall -I. \
        -c main.c -o main.o
    arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -g -O0 -Wall -I. \
        -c sensor_sampling.c -o sensor_sampling.o
    # ... repeat for other .c files if they were separate

    arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -g -O0 -Wall \
        -T linker_script.ld -o firmware.elf \
        main.o sensor_sampling.o # ... and other object files
    ```

    *   `-mcpu=cortex-m4 -mthumb`: Specify the target architecture (adjust as per your MCU).
    *   `-g -O0`: Include debug information and no optimization (for development).
    *   `-Wall`: Enable all warnings.
    *   `-I.`: Include current directory for headers.
    *   `-c`: Compile only, do not link.
    *   `-T linker_script.ld`: Specify your MCU's linker script.
    *   `firmware.elf`: Output executable file.

3.  **Linker Script**: A `linker_script.ld` specific to your microcontroller is required for linking.
4.  **Hardware Abstraction Layer (HAL)**: The dummy `ADS1115_Init`, `ADS1115_ReadRawData`, `TIMER_DRV_Init`, and `TIMER_DRV_Get_Uptime_ms` functions need to be replaced with actual implementations using your microcontroller's HAL (e.g., STM32 HAL, NRFX, etc.) and I2C driver for the ADS1115.
5.  **Interrupt Configuration**: The timer ISR (e.g., `TIMx_IRQHandler` for STM32) must be configured to call `SENSOR_SAMPLING_Timer_ISR()`.