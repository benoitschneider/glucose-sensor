/* Linker script for nRF52832_xxAA */
/* This is a simplified linker script. In a real project, you would use one from the SDK. */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x80000 /* 512KB for nRF52832, 0x80000 */
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 0x10000 /* 64KB for nRF52832, 0x10000 */
}

/* Define output sections */
SECTIONS
{
  .text :
  {
    KEEP(*(.vectors))
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  .data :
  {
    . = ALIGN(4);
    *(.data*)
  } > RAM AT > FLASH

  .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    __bss_end__ = .;
  } > RAM

  .heap :
  {
    __heap_start__ = .;
    . = . + __heap_size__; /* Defined in CMakeLists.txt */
    __heap_end__ = .;
  } > RAM

  .stack_dummy :
  {
    . = . + __stack_size__; /* Defined in CMakeLists.txt */
  } > RAM

  /* Set stack top address directly into a symbol */
  __StackTop = ORIGIN(RAM) + LENGTH(RAM);
  __StackLimit = __StackTop - __stack_size__;

  PROVIDE(__exidx_start = .);
  PROVIDE(__exidx_end = .);

  /* Check if stack and heap don't overlap */
  ASSERT((__StackLimit >= __heap_end__), "Error: Stack and Heap overlap!");
}
