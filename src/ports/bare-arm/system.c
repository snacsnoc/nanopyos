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
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "bare-arm/system.h"

// Simulated UART status flags
volatile bool uart_txe_simulated = true; // Assume TXE is initially true (ready to send)
volatile bool uart_rxne_simulated = false; // RXNE initially false (no data to read)


typedef struct {
    volatile uint32_t DR;   // Data Register at offset 0x00 from UART_BASE
    volatile uint32_t SR;   // Status Register at offset 0x04 from UART_BASE
    volatile uint32_t BRR;
    volatile uint32_t CR1;

    // Simulated functions for setting/getting status flags
    void (*set_txe)(bool state);

    void (*set_rxne)(bool state);
} periph_uart_t;

#define UART ((periph_uart_t *)0x09000000)

// Bit masks for the Status Register (SR)

// RXNE: Receive data register not empty
#define UART_SR_RXNE (1U << 5)
// TXE: Transmit data register empty
#define UART_SR_TXE  (1U << 7)


extern uint64_t _estack, _sidata, _sdata, _edata, _sbss, _ebss, _start;

void main(void);


// Very simple ARM vector table.
const uint64_t isr_vector[] __attribute__((section(".isr_vector"))) = {
        (uint64_t) & _estack,
        (uint64_t) & _start,
};

// Simulated RXNE and TXE
void uart_set_txe_simulated(bool state) {
    uart_txe_simulated = state;
}

void uart_set_rxne_simulated(bool state) {
    uart_rxne_simulated = state;
}

// Initializing the UART structure with simulated functions
void uart_init() {
    printc("\n\ninit uart flags\n\n");
    UART->set_txe = uart_set_txe_simulated;
    UART->set_rxne = uart_set_rxne_simulated;
}

// Simulated UART send character function
void uart_send_char(char c) {
    while (!uart_txe_simulated) {
        // Wait for TXE to be ready to send
        printc("waiting for data\n");
    }
    UART->DR = (uint32_t) c;
    // Simulate the TXE flag being cleared after sending a character
    UART->set_txe(false);
    // Simulate the TXE flag being set again (ready for next character)
    UART->set_txe(true);
}

// Simulated UART receive character function
char uart_receive_char() {
    UART->set_rxne(true);
    printc("init waiting....\n");
    while (!uart_rxne_simulated) {
        printc("waiting for data\n");
        continue;
    }
    //char receivedChar = (char) UART->DR; // Read char from data register
    // Simulate the RXNE flag being cleared after reading a character
    UART->set_rxne(false);
    return (char) UART->DR;
}

// Reading a character from UART
int uart_read_char(void) {
    while (!uart_has_data()) {} // Wait for data
    return (uint32_t)(UART->DR & 0xFF); // Read and return received data
}


int mp_hal_stdin_rx_chr(void) {
    // Wait until data is available
    while (!(UART->SR & UART_SR_RXNE)) {

        continue; // Wait while the receive data register is empty.
    }
    return (int) (UART->DR & 0xFF); // Read the received character.
}

void debug_print(const char *message, uint32_t value) {
    printf("%s: 0x%X\n", message, value);
}


// Directly write the character to the UART data register
void uart_putc(char c) {

    // Wait for the transmit data register to be empty
    //while (!(UART->SR & UART_SR_TXE)) {}

    // Write the character to the Data Register
    UART->DR = (uint32_t) c;
    UART->SR &= ~UART_SR_TXE; // Force the TXE flag to a clear state
    UART->SR |= UART_SR_TXE; // Force the TXE flag to a set state
}

void mp_hal_stdout_tx_strn(const char *s) {
    while (*s != '\0') {
        pputchar(*s);
        s++;
    }
}


int uart_has_data(void) {
    return UART->SR & UART_SR_RXNE;
}


void pputchar(char c) { UART->DR = (uint32_t) c; }

void print(const char *s) {
    while (*s != '\0') {

        uart_send_char(*s);
        s++;
    }
}


// Send string of given length
void printc(const char *s) {
    while (*s) {
        pputchar(*s++);
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
        pputchar(*buf_ptr++);
    }
}