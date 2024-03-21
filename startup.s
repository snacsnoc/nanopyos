.section .text
.global _start
.global Reset_Handler

_start:
Reset_Handler:
    /* Set the stack pointer */
    ldr x0, =_estack
    mov sp, x0

    /* Copy .data section from flash to RAM */
    ldr x1, =_sidata
    ldr x2, =_sdata
    ldr x3, =_edata
copy_loop:
    cmp x2, x3
    b.cc copy_done
    ldrb w0, [x1], #1
    strb w0, [x2], #1
    b copy_loop
copy_done:

    /* Zero out .bss section */
    ldr x2, =_sbss
    ldr x3, =_ebss
zero_loop:
    cmp x2, x3
    b.cc zero_done
    mov w0, #0
    strb w0, [x2], #1
    b zero_loop
zero_done:

    /* Call main application code or further initialization here */
    bl bare_main

    /* If main returns, enter an infinite loop */
infinite_loop:
    b infinite_loop
