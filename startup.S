.section .text
.global _start
.global Reset_Handler

_start:
Reset_Handler:
    /* Set the stack pointer */
    ldr x0, =_estack
    mov sp, x0

    // Copy .data section from flash to RAM
   // ldr x1, =_sidata   // Start of .data in flash
    ldr x2, =_sdata    // Start of .data in RAM
    ldr x3, =_edata    // End of .data in RAM
copy_loop:
    cmp x2, x3
    b.hs copy_done
    ldrb w4, [x1], #1  // Use w4 to avoid "expected an integer" error
    strb w4, [x2], #1
    b copy_loop
copy_done:

    /* Zero out .bss section */
    ldr x2, =_sbss
    ldr x3, =_ebss
zero_loop:
    cmp x2, x3
    b.hs zero_done
    mov w4, #0         // Zero out w4 register
    strb w4, [x2], #1
    b zero_loop
zero_done:

    bl main       // Branch to main function

    // Infinite loop
infinite_loop:
    wfi                // Wait for interrupt
    b infinite_loop

