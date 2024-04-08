/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "bare-arm/system.h"

typedef struct {
    volatile uint32_t DR;   // Data Register at offset 0x00 from UART_BASE
    volatile uint32_t SR;   // Status Register at offset 0x04 from UART_BASE
    volatile uint32_t BRR;
    volatile uint32_t CR1;

} periph_uart_t;

#define UART ((periph_uart_t *)0x09000000)


// RXNE: Receive data register not empty
#define UART_SR_RXNE (1U << 5)
// TXE: Transmit data register empty
#define UART_SR_TXE  (1U << 7)


extern uint64_t _estack, _start;

void main(void);


// Very simple ARM vector table.
const uint64_t isr_vector[] __attribute__((section(".isr_vector"))) = {
        (uint64_t) & _estack,
        (uint64_t) & _start,
};



// Init UART structures
void uart_init() {
    printc("\n\ninit uart flags\n\n");
}

// Write a character out to the UART.
void uart_write_char(int c) {
    // Wait for TXE, then write the character.
//    while ((UART4->SR & (1 << 7)) == 0) {
//    }
    UART->DR = (uint64_t)c;
}


// Reading a character from UART
int uart_read_char(void) {
    while (!uart_has_data()) {} // Wait for data
    return (uint32_t)(UART->DR & 0xFF); // Read and return received data
}


// Receive single character
int mp_hal_stdin_rx_chr(void) {
    unsigned char c = 0;
#if MICROPY_MIN_USE_STDOUT
    int r = read(STDIN_FILENO, &c, 1);
    (void)r;
#endif
    return c;
}


int uart_has_data(void) {
    return UART->SR & UART_SR_RXNE;
}

void printc(const char *s) {
    while (*s != '\0') {

        uart_write_char(*s);
        s++;
    }
}


void printc_hex(unsigned long value) {
    const char hex_chars[] = "0123456789ABCDEF";
    char buffer[19]; // Long enough for 64-bit hexadecimal + "0x" prefix + null terminator
    char *buf_ptr = &buffer[18];
    *buf_ptr = '\0';

    do {
        *(--buf_ptr) = hex_chars[value % 16];
        value /= 16;
    } while (value != 0);


    *(--buf_ptr) = 'x';
    *(--buf_ptr) = '0';


    while (*buf_ptr) {
        uart_write_char(*buf_ptr++);
    }
}



// Send string of given length to stdout, converting \n to \r\n.
void mp_hal_stdout_tx_strn(const char *str, size_t len) {
    while (len--) {
        if (*str == '\n') {
            uart_write_char('\r');
        }
        uart_write_char(*str++);
    }
}