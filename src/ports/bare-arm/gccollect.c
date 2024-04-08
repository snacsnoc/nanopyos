/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
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

#include "py/gc.h"

#include "shared/runtime/gchelper.h"

#if MICROPY_ENABLE_GC
void gc_collect(void) {
    extern void *_estack;
    uintptr_t stack_top = (uintptr_t)&_estack;

    // Use inline assembly to get the current stack pointer for ARM64
    uintptr_t stack_bottom;
    asm volatile ("mov %0, sp" : "=r" (stack_bottom) ::);

    gc_collect_start();
    // Be implicit about getting roots from the stack and registers
    gc_collect_root((void**)&stack_bottom, (stack_top - stack_bottom) / sizeof(mp_uint_t));
    //gc_helper_collect_regs_and_stack();
    gc_collect_end();
    gc_dump_info(&mp_plat_print);
}
#endif

