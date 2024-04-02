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
#include <string.h>
#include "system.h"


//#define UART4 ((periph_uart_t *) 0x09000000)
#define UART_BASE ((volatile uint8_t *) 0x09000000)

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
} periph_uart_t;

#define UART ((periph_uart_t *)UART_BASE)

extern uint64_t _estack, _sidata, _sdata, _edata, _sbss, _ebss, _start;

void main(void);


// Very simple ARM vector table.
const uint64_t isr_vector[] __attribute__((section(".isr_vector"))) = {
        (uint64_t) & _estack,
        (uint64_t) & _start,
};

// Directly write the character to the UART data register
void uart_write_char(int c) {
    *UART_BASE = c;
}

// Reading a character from UART
int uart_read_char(void) {
    // Wait for character to be available in the receive data register
    while (!(UART->SR & (1 << 5))) {}
    return (int)(UART->DR);
}

// Send string of given length to stdout, converting \n to \r\n.
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    while (len--) {
        if (*str == '\n') {
            uart_write_char('\r');
        }
        uart_write_char(*str++);
    }
}
