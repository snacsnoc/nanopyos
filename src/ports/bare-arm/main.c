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

#include <string.h>
#include "py/compile.h"
#include "py/runtime.h"



const char *text;
volatile uint8_t *uart = (uint8_t *) 0x09000000;


static const char *demo_single_input =
        "print('hello world!', list(x + 1 for x in range(10)), end='eol\\n')";

static const char *demo_complex_input =
        "def calculate_primes(limit):\n"
        "    primes = []\n"
        "    for num in range(2, limit + 1):\n"
        "        for prime in primes:\n"
        "            if num % prime == 0:\n"
        "                break\n"
        "        else:\n"
        "            primes.append(num)\n"
        "    return primes\n"
        "\n"
        "limit = 30\n"
        "primes = calculate_primes(limit)\n"
        "print('Primes up to {}:'.format(limit))\n"
        "print(primes)\n"
        "\n"
        "squares = [x**2 for x in range(10)]\n"
        "print('Squares of numbers from 0 to 9:')\n"
        "print(squares)\n"
        "\n"
        "for i in range(10):\n"
        "    if i % 2 == 0:\n"
        "        print('{} is even'.format(i))\n"
        "    else:\n"
        "        print('{} is odd'.format(i))";


extern int main(void) __attribute__((visibility("default")));

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
        print("Exception caught");
        // Uncaught exception: print it out.
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}




// Main entry point: initialise the runtime and execute demo strings.
int main(void) {
    print("Boots");
    mp_init();
    do_str(demo_single_input, MP_PARSE_SINGLE_INPUT);
    do_str(demo_complex_input, MP_PARSE_FILE_INPUT);
    mp_deinit();
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
