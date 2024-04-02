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

#include <stdint.h>
#include "sys/types.h"

#define MICROPY_BANNER_MACHINE "CustomMachine"

// Enable all common features (large/external flash, rp2, unix)
#define MICROPY_CONFIG_ROM_LEVEL_FULL_FEATURES (40)

// Compiler configuration
#define MICROPY_ENABLE_COMPILER                 (1)

// Python internal features
#define MICROPY_MALLOC_USES_ALLOCATED_SIZE (1)
#define MICROPY_PY_EXEC             (1)
#define MICROPY_MEM_STATS           (1)
#define MICROPY_ENABLE_GC           (1)
#define MICROPY_PY_GC               (1)
#define MICROPY_GC_SPLIT_HEAP_AUTO  (1)
#define MICROPY_KBD_EXCEPTION       (1)
#define MICROPY_HELPER_REPL         (1)
#define MICROPY_USE_READLINE        (1)
#define MICROPY_LONGINT_IMPL        (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL          (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_WARNINGS            (1)
#define MICROPY_ERROR_REPORTING     (MICROPY_ERROR_REPORTING_NONE)


#define MICROPY_GC_STACK_ENTRY_TYPE size_t

// Type definitions for the specific machine
typedef int64_t mp_int_t;
typedef uint64_t mp_uint_t;
typedef long mp_off_t;

// Need to provide a declaration/definition of alloca()
#include <alloca.h>
