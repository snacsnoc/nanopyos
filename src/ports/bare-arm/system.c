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

#define UART_BASE 0x09000000

typedef struct {
    volatile uint32_t DR;   // Data Register at offset 0x00 from UART_BASE
    volatile uint32_t _reserved0[5]; // Reserved space
    volatile uint32_t FR;   // Flag Register at offset 0x018 from UART_BASE
    volatile uint32_t _reserved1[1]; // Reserved space
    volatile uint32_t IBRD; // Integer Baud Rate Register at offset 0x024
    volatile uint32_t FBRD; // Fractional Baud Rate Register at offset 0x028
    volatile uint32_t LCR;  // Line Control Register at offset 0x02c
    volatile uint32_t CR;   // Control Register at offset 0x030
    volatile uint32_t _reserved2[1]; // Reserved space
    volatile uint32_t IMSC; // Interrupt Mask Set/Clear Register at offset 0x038
    volatile uint32_t _reserved3[2]; // Reserved space
    volatile uint32_t DMACR; // DMA Control Register at offset 0x048

} periph_uart_t;
// https://developer.arm.com/documentation/ddi0183/g/programmers-model/summary-of-registers
#define UART ((periph_uart_t *)UART_BASE)

// Receive FIFO Empty
#define UARTFR_RXFE (1U << 4)
// Transmit FIFO Full
#define UARTFR_TXFF (1U << 5)
// UART Busy
#define UARTFR_BUSY (1U << 3)

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

// Check if UART is ready to send data
static int uart_can_send(void) {
    // Transmit FIFO not full and UART not busy
    return !(UART->FR & (UARTFR_TXFF | UARTFR_BUSY));
}

// Write a character out to the UART.
static void uart_write_char(int c) {
    // Wait for TXFF, then write the character.
    while (!uart_can_send()) {} // Wait for transmitter to be ready
    UART->DR = (uint64_t) c;
}

int uart_has_data(void) {
    // Receive FIFO not empty
    return !(UART->FR & UARTFR_RXFE);
}


// Reading a character from UART
int uart_read_char(void) {
    while (!uart_has_data()) {} // Wait for data
    return UART->DR & 0xFF; // Read and return received data
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