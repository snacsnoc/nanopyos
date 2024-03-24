# Minimal Kernel for ARM64

This is a minimal kernel designed for ARM64. It includes basic functionality such as memory management, exception handling, and console output. 
The kernel is written in C and assembly, with a focus on simplicity.

## Files
- `system.h`: Contains function declarations for UART communication.
- `startup.S`: Assembly startup code that initializes the stack and zeroes out the BSS section, branches to the main function.
- `main_simple.c`: Main C file containing the kernel's entry point, exception handling, console output functions, and a simple printf implementation.
- `system_simple.c`: Contains an ARM vector table for exception handling.
- `linker-simple.ld`: Linker script that defines memory regions, sections, and their respective locations in the RAM. 
