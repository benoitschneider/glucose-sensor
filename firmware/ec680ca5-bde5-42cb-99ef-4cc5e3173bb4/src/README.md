# Integrate the chosen filter into the glucose processing pipeline and provide a filtered output.

## Build Instructions

To compile this code, you would typically use an ARM GCC toolchain (e.g., `arm-none-eabi-gcc`).

Assuming a simple embedded project structure, you can compile the `.c` files and link them:

1.  **Compile:**
    `arm-none-eabi-gcc -c main.c -o main.o -Wall -Wextra -std=c99 -mcpu=<your_cortex_m_core> -mthumb`
    `arm-none-eabi-gcc -c glucose_filter.c -o glucose_filter.o -Wall -Wextra -std=c99 -mcpu=<your_cortex_m_core> -mthumb`

2.  **Link:**
    `arm-none-eabi-gcc main.o glucose_filter.o -o firmware.elf -T <your_linker_script.ld> -nostdlib -Wl,-Map=firmware.map`

Replace `<your_cortex_m_core>` (e.g., `cortex-m4`) and `<your_linker_script.ld>` with values appropriate for your target microcontroller.

For a host-side test (e.g., on Linux/Windows), you can compile with a standard GCC:
`gcc main.c glucose_filter.c -o glucose_app -Wall -Wextra -std=c99`