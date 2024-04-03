/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2021 Damien P. George
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
#define BYTES_PER_WORD 8

#include <string.h>
#include <stddef.h>
#include "bare-arm/input.h"
#include "bare-arm/mphalport.h"
#include "bare-arm/mpconfigport.h"
#include "bare-arm/system.h"

#include "py/builtin.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/stackctrl.h"
#include "py/repl.h"

#include "shared/runtime/gchelper.h"
#include "shared/runtime/pyexec.h"
#include "shared/readline/readline.h"


const char *text;
volatile uint8_t *uart = (uint8_t *) 0x09000000;
volatile uint8_t *uart_status = (uint8_t *)0x09000004;


// Prototype for hardware-specific UART read function
// Returns -1 if no data, or the read byte
int uart_read_byte(void);

long heap_size = 1024 * 1024 * (sizeof(mp_uint_t) / 4);

// Python heap
size_t gc_get_max_new_split(void) {
    return 128 * 1024 * 1024;
}

extern int main(void) __attribute__((visibility("default")));
extern char _sheap;
extern char _eheap;

int putchar(int c);


void print(const char *s) {
    while (*s != '\0') {
        putchar(*s);
        s++;
    }
}
// Function to check if UART has received data
int uart_has_data(void) {
    return *uart_status & 0x01;
}

// Function to receive a byte from UART
int uart_receive_byte(void) {
    if (uart_has_data()) {
        // Read a byte from UART data register and return it
        return (int)(*uart);
    } else {
        return -1;
    }
}

// Custom read function to replace Unix `read`
// Attempts to read `count` bytes into `buf` from file descriptor `fd`
// Since this is a bare-metal system, `fd` is ignored
ssize_t read(int fd, void *buf, size_t count) {
    unsigned char *pbuf = (unsigned char *)buf;
    size_t bytes_read = 0;

    for (size_t i = 0; i < count; i++) {
        int byte = uart_read_byte();
        if (byte == -1) {
            // No more data available
            break;
        }
        pbuf[i] = (unsigned char)byte;
        bytes_read++;
    }

    return bytes_read;
}

// Dummy implementation of uart_read_byte
int uart_read_byte(void) {
    // Check if UART has data and return the byte
    // Return -1 if no data is available
    return -1;
}

// Receive single character, blocking until one is available.
/*
int mp_hal_stdin_rx_chr(void) {
    unsigned char c = 0;
    int r = read(STDIN_FILENO, &c, 1);
    (void)r;
    return c;
}
 */
int mp_hal_stdin_rx_chr(void) {
    int byte;
    while ((byte = uart_receive_byte()) < 0) {
        // TODO: include a small delay here to prevent a tight polling loop,
    }
    return byte;
}


// Main entry point: initialise the runtime
int main(void) {
    print("Boots");
    mp_stack_ctrl_init();
    mp_stack_set_limit(40000 * (BYTES_PER_WORD / 4));

    gc_init(&_sheap, &_eheap);
    mp_init();
    mp_hal_set_interrupt_char(CHAR_CTRL_C);
    pyexec_friendly_repl();
    gc_sweep_all();
    mp_deinit();
    return 0;
}


// Called if an exception is raised outside all C exception-catching handlers.
void nlr_jump_fail(void *val) {
    for (;;) {
    }
}

#if MICROPY_ENABLE_COMPILER
void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}
#endif

// There is no filesystem so stat'ing returns nothing.
mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

// There is no filesystem so opening a file raises an exception.
mp_lexer_t *mp_lexer_new_from_file(qstr filename) {
    mp_raise_OSError(MP_ENOENT);
}



// Send string of given length
mp_uint_t mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    mp_uint_t ret = len;
#if MICROPY_MIN_USE_STDOUT
    int r = write(STDOUT_FILENO, str, len);
    if (r >= 0) {
        // in case of an error in the syscall, report no bytes written
        ret = 0;
    }
#elif MICROPY_MIN_USE_STM32_MCU
    while (len--) {
        // wait for TXE
        while ((USART1->SR & (1 << 7)) == 0) {
        }
        USART1->DR = *str++;
    }
#endif
    return ret;
}

#ifndef NDEBUG
// Used when debugging is enabled.
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    for (;;) {
    }
}
#endif
