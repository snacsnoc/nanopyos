/* Linker script for micropython build */

MEMORY
{
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

ENTRY(_start)

SECTIONS
{
    . = ORIGIN(RAM);

    /* Text and rodata */
    .text : {
        *(.text.startup)
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > RAM

    /* Data section */
    .data : {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > .text

    /* BSS Section */
    .bss : {
        _sbss = .;
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    /* Heap Section */
    .heap : {
        _sheap = .;
        . += 1M;
        _eheap = .;
        . = ALIGN(4);
    } > RAM

    /* Stack Section */
    .stack : {
        . += 64K;
        _estack = .;
        . = ALIGN(16);
    } > RAM

    /* Set _heap_start to current location */
    .heap_start : {
        _heap_start = .;
    } > RAM

    /* Provide _estack symbol for the startup code */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
