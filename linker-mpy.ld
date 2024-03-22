ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x40080000, LENGTH = 8M
}



SECTIONS
{
    . = ORIGIN(RAM);
    /*. = 0x80010000;*/
    .isr_vector :
    {
        . = ALIGN(8);
        KEEP(*(.isr_vector))
        . = ALIGN(8);
    } > RAM
    /* Text and rodata */
    .text : {
        *(.text.startup)
        *(.text*)
        *(.rodata*)
        . = ALIGN(8);
    } > RAM

    /* Data section */
    .data : {
        _sdata = .;
        *(.data*)
        . = ALIGN(8);
        _edata = .;
    } > RAM

    /* BSS Section */
    .bss : {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        _ebss = .;
    } > RAM
     _end = .; /* Mark end of used memory */

    /* Heap Section */
    .heap : {
        _sheap = .;
        _heap_start = .;
        . += 1M;
        _eheap = .;
        . = ALIGN(8);
    } > RAM

    /* Stack Section */
    .stack : {
        . += 64K;
        . = ALIGN(16);
    } > RAM

    /* Provide _estack symbol for the startup code */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
