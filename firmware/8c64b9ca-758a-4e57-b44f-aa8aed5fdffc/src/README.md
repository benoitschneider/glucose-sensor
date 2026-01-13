# Implement a configurable IIR low-pass filter for glucose readings.

## Build Instructions

To compile this code, you will need a C compiler, preferably `gcc-arm-none-eabi` for embedded targets, or `gcc` for a desktop simulation.

1.  **Save the files**: Save `main.c`, `glucose_filter.c`, `glucose_filter.h`, `config.h`, and `nvm.c` into a single directory.
2.  **Compile (for desktop simulation)**: 
    `gcc main.c glucose_filter.c nvm.c -o glucose_filter_test -lm -std=c99 -Wall -Wextra`
    *   `-lm` is required for `math.h` functions like `tanf` and `M_PI`.
    *   `-std=c99` ensures C99 standard features are available.
    *   `-Wall -Wextra` enable useful warnings.
3.  **Run**: `./glucose_filter_test`

4.  **Compile (for ARM embedded target using `gcc-arm-none-eabi`)**:
    `arm-none-eabi-gcc -c main.c glucose_filter.c nvm.c -o main.o -o glucose_filter.o -o nvm.o -mcpu=<your_cpu_arch> -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -std=c99 -Wall -Wextra -I.`
    `arm-none-eabi-gcc main.o glucose_filter.o nvm.o -o glucose_filter.elf -mcpu=<your_cpu_arch> -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -T <linker_script.ld> -lm`
    *   Replace `<your_cpu_arch>` with your specific ARM core (e.g., `cortex-m4`).
    *   Replace `<linker_script.ld>` with your project's linker script.
    *   `-mfloat-abi=softfp -mfpu=fpv4-sp-d16` are examples for Cortex-M4 with FPU; adjust for your specific hardware.
    *   `-I.` ensures the compiler looks for headers in the current directory.

This setup provides a basic executable for testing the filter logic. For an actual embedded project, integration with a specific microcontroller's HAL and build system (e.g., Makefiles, CMake) would be necessary.