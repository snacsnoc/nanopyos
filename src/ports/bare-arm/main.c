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
#include "bare-arm/input.h"
#include "bare-arm/mphalport.h"
#include "bare-arm/system.h"

#include "py/compile.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "py/repl.h"
#include "py/stackctrl.h"
#include "py/mphal.h"
#include "py/gc.h"
#include "py/lexer.h"
#include "pyexec.h"

#include "shared/readline/readline.h"


const char *text;
volatile uint8_t *uart = (uint8_t *) 0x09000000;

long heap_size = 1024 * 1024 * (sizeof(mp_uint_t) / 4);

// Python heap
size_t gc_get_max_new_split(void) {
    return 128 * 1024 * 1024;
}

extern int main(void) __attribute__((visibility("default")));
extern char _sheap;
extern char _eheap;

int putchar(int c) {
    *uart = (uint8_t)c;
    return c;
}


void print(const char *s) {
    while (*s != '\0') {
        putchar(*s);
        s++;
    }
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

    return 0;
}

// Called if an exception is raised outside all C exception-catching handlers.
void nlr_jump_fail(void *val) {
    for (;;) {
    }
}

#ifndef NDEBUG
// Used when debugging is enabled.
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    for (;;) {
    }
}
#endif
