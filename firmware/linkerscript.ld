/*
 * Default linker script for nRF52832 (Cortex-M4F)
 * This is a basic example and might need adjustments based on specific project needs
 * and SoftDevice requirements.
 */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K /* 512KB Flash */
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 64K  /* 64KB RAM */
}

SECTIONS
{
  .text :
  {
    KEEP(*(.vectors))
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  .ARM.exidx :
  {
    *(.ARM.exidx*)
  } > FLASH

  .ARM.extab :
  {
    *(.ARM.extab*)
  } > FLASH

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT > FLASH

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  .heap :
  {
    . = ALIGN(4);
    _sbrk = .;
    _eheap = ORIGIN(RAM) + LENGTH(RAM);
  } > RAM

  .stack :
  {
    . = ALIGN(8);
    _estack = ORIGIN(RAM) + LENGTH(RAM);
  } > RAM

  /* Provide global symbols for the start and end of RAM */
  _sram = ORIGIN(RAM);
  _eram = ORIGIN(RAM) + LENGTH(RAM);
}

/* Entry Point */
ENTRY(Reset_Handler)

/* Define the stack top for the startup code */
_stack_top = ORIGIN(RAM) + LENGTH(RAM);

/* Define symbols for the linker to use for copying data from FLASH to RAM */
_sidata = LOADADDR(.data);
