.section .text
.global _start

_start:
    @ Arbitrary address
    mov sp, #0x40000

    bl main

    @ Infinite loop to prevent the kernel from "falling off" after main returns
hang:
    b hang
