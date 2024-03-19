SECTIONS
{
    /* Set the starting address for the sections */
    . = 0x80000;

    /* Text section for code */
    .text : {
        *(.text*)
    }

    /* Read-only data section */
    .rodata : {
        *(.rodata*)
    }

    /* Data section for initialized data */
    .data : {
        *(.data*)
    }

    /* BSS section for uninitialized data */
    .bss : {
        *(.bss*)
    }
}
