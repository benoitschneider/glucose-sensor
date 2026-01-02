/* This is a placeholder linker script for nRF52832.
 * In a real project, you would copy this from the nRF5 SDK.
 * Example path in SDK: components/toolchain/gcc/nrf52_flash.ld
 */

SEARCH_DIR(.)
GROUP(-lc -lgcc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x80000 /* 512K */
  RAM (rwx) :  ORIGIN = 0x20000000, LENGTH = 0x10000 /* 64K */
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

  .ARM.extab : ALIGN(4)
  {
    *(.ARM.extab*)
  } > FLASH

  .ARM.exidx : ALIGN(4)
  {
    *(.ARM.exidx*)
  } > FLASH

  .eh_frame : ALIGN(4)
  {
    *(.eh_frame)
  } > FLASH

  . = ALIGN(4);
  _etext = .;

  .data : AT (_etext)
  {
    _data = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM

  .bss (NOLOAD) :
  {
    _bss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  .heap (NOLOAD) :
  {
    _heap_start = .;
    . = . + __HEAP_SIZE;
    _heap_end = .;
  } > RAM

  .stack (NOLOAD) :
  {
    _stack_end = .;
    . = . + __STACK_SIZE;
    _stack_start = .;
  } > RAM

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .comment       0 : { *(.comment) }
  .debug_abbrev  0 : { *(.debug_abbrev) }
  .debug_info    0 : { *(.debug_info) }
  .debug_aranges 0 : { *(.debug_aranges) }
  .debug_line    0 : { *(.debug_line) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_frame   0 : { *(.debug_frame) }
  .debug_str     0 : { *(.debug_str) }
  .debug_loc     0 : { *(.debug_loc) }
  .debug_macinfo 0 : { *(.debug_macinfo) }
}
