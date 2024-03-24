.section .text
.global _start

_start:

    /* Set the stack pointer */
    ldr x0, =_estack
    mov sp, x0


    /* Initialize .bss section (zero-initialize) */
    ldr x1, =_sbss
    ldr x2, =_ebss


zero_loop:
    cmp x1, x2
    b.ge zero_done
    str xzr, [x1], #8   // Store zero and increment address by 8 bytes
    b zero_loop

zero_done:

    bl main       // Branch to main function
    b . // Infinite loop



