/* Linker script for nRF52832_xxAA */
/* This is a simplified linker script. For a full SDK-compliant script, refer to Nordic's examples. */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x80000 /* 512KB */
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x10000  /* 64KB */
}

SECTIONS
{
  .text : {
    KEEP(*(.vectors))
    *(.text*)
    *(.rodata*)
  } > FLASH

  .data : {
    *(.data*)
  } > RAM AT> FLASH

  .bss : {
    *(.bss*)
    *(COMMON)
  } > RAM

  .heap : {
    __HeapBase = .;
    . = . + 0x1000; /* 4KB heap */
    __HeapLimit = .;
  } > RAM

  .stack : {
    __StackLimit = .;
    . = . + 0x1000; /* 4KB stack */
    __StackTop = .;
  } > RAM

  /* Provide global symbols for the linker */
  _estack = ORIGIN(RAM) + LENGTH(RAM); /* Top of RAM */
  _sdata = LOADADDR(.data);
  _edata = ADDR(.data) + SIZEOF(.data);
  _sbss = ADDR(.bss);
  _ebss = ADDR(.bss) + SIZEOF(.bss);

} 
