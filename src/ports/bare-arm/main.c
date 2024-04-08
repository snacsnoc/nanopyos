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
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#if MICROPY_MIN_USE_STDOUT
//used for write()
#include <unistd.h>
#endif

#include "bare-arm/mphalport.h"
#include "bare-arm/mpconfigport.h"
#include "bare-arm/system.h"

#include "py/builtin.h"
#include "py/runtime.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/stackctrl.h"
#include "py/repl.h"
#include "py/obj.h"


#include "shared/runtime/gchelper.h"
#include "shared/runtime/pyexec.h"
#include "shared/readline/readline.h"

const char *text;


extern int main(void) __attribute__((visibility("default")));

extern char _sheap;
extern char _eheap;
extern char _estack;


#if MICROPY_ENABLE_COMPILER
static const char *demo_single_input =
        "print('hello world!', list(x + 1 for x in range(10)), end='eol\\n')";

static void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        // Compile, parse and execute the given string.
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // Uncaught exception: print it out.
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}
#endif



int mem_test(void) {
    printc("Heap start: ");
    printc_hex((unsigned long) &_sheap);
    printc("\nHeap end: ");
    printc_hex((unsigned long) &_eheap);
    printc("\nStack end (top): ");
    printc_hex((unsigned long) &_estack);
    printc("\n");
    void *test_alloc = malloc(48);
    if (test_alloc == NULL) {
        printc("Failed to allocate memory\n");
    } else {
        printc("Memory allocation successful\n\n");
        m_free(test_alloc, 48);
    }
    return 0;
}


// wrapper for gc_init with debug prints
void gc_init_debug(void *start, void *end) {
    if (!start || !end || start >= end) {
        printc("Invalid heap boundaries.\n");
        return;
    }
    gc_init(start, end);
    printc("GC init called with boundaries: Start: ");
    printc_hex((unsigned long) start);
    printc(" End: ");
    printc_hex((unsigned long) end);
    printc("\n");
}

// Main entry point: initialise the runtime
int main(void) {
    printc("Boots\n\n");
    //uart_init();
    mp_stack_ctrl_init();

    mp_stack_set_limit(64 * 1024);

    gc_init(&_sheap, &_eheap);
    //gc_init_debug(&_sheap, &_eheap);
    mp_init();

    mp_hal_set_interrupt_char(CHAR_CTRL_C);
    do_str(demo_single_input, MP_PARSE_SINGLE_INPUT);
#if MICROPY_REPL_EVENT_DRIVEN
    pyexec_event_repl_init();
    for (;;) {
        int c = uart_read_char();
        if (pyexec_event_repl_process_char(c)) {
            break;
        }
    }
#else
    readline_init0();

    pyexec_friendly_repl();
#endif

    gc_sweep_all();

    mp_deinit();


    return 0;
}

// Called if an exception is raised outside all C exception-catching handlers.
void nlr_jump_fail(void *val) {
    printc("nlr jump fail");
    for (;;) {
    }
}


// There is no filesystem so stat'ing returns nothing.
mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

// There is no filesystem so opening a file raises an exception.
mp_lexer_t *mp_lexer_new_from_file(qstr filename) {
    return 0;
}


#ifndef NDEBUG
// Used when debugging is enabled.
void MP_WEAK

__assert_func(const char *file, int line, const char *func, const char *expr) {
    for (;;) {
    }
}

#endif

void NORETURN

__fatal_error(const char *msg) {
    while (1) { ;
    }
}