
/*
  FICR  (r)   : ORIGIN = 0x10000000
  UICR  (r)   : ORIGIN = 0x10001000
  APB   (rw)  : ORIGIN = 0x40000000
  AHB   (rw)  : ORIGIN = 0x50000000
  PPB   (rw)  : ORIGIN = 0xE0000000
*/

SECTIONS
{
    .ivt :
    {
        . = ALIGN(4);
        KEEP(*(.vectors))
        . = ALIGN(4);
    } > FLASH

    .text :
    {
        /* interrupt vector table */
        /* sdk/nrf51_startup.o(.vectors) */

        /* program code */
        code_begin = .;
        *(.text*)
        code_end = .;

        /* initialized constants */
        *(.rodata*)

	/* when linking standard library functions, sections .init and .fini are added */
        KEEP (*(.init))
        KEEP (*(.fini))

        /* section to support exception handling for C++ */
        *(.eh_frame*)

        text_end = .;
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } > FLASH

    __end_code = .;

    /*
     * Initialized variables (non-constant)
     * will be copied to RAM upon startup
     * https://en.wikipedia.org/wiki/Data_segment
     *
     * VMA is immediately after .text section
     * LMA is the beginning of RAM
     */
    .data :
    AT ( ADDR (.text) + SIZEOF (.text) )
    {
        data_begin = .;
        *(.data*)
        data_end = .;
    } > RAM

    /*
     * "Better save space" for uninitialized variables and constants
     * https://en.wikipedia.org/wiki/.bss
     */
    .bss :
    {
        __bss_start__ = .;
        *(.bss*)
        __bss_end__ = .;
    } > RAM

    .heap :
    {
        heap_begin = .;
        *(.heap*)
        . = . + heap_size;
        heap_end = .;
    } > RAM

    stack_begin = .;
    stack_end = ORIGIN(RAM) + LENGTH(RAM);

    end = .;
}
