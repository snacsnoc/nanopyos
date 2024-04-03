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
//#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)


// Compiler configuration
#define MICROPY_ENABLE_COMPILER                 (1)


// Ensure builtinimport.c works with -m.
#define MICROPY_MODULE_OVERRIDE_MAIN_IMPORT (1)


#define MICROPY_ENABLE_EXTERNAL_IMPORT (1)

#define MICROPY_ALLOC_PATH_MAX            (256)

// Python internal features
#define MICROPY_MALLOC_USES_ALLOCATED_SIZE (1)
#define MICROPY_PY_EXEC             (1)
#define MICROPY_MEM_STATS           (1)
#define MICROPY_ENABLE_GC           (1)
//#define MICROPY_PY_GC               (0)
//#define MICROPY_GC_SPLIT_HEAP_AUTO  (1)
#define MICROPY_REPL_INFO           (1)

#define MICROPY_KBD_EXCEPTION       (1)
#define MICROPY_HELPER_REPL         (1)

#define MICROPY_NLR_AARCH64 (1)
#define MICROPY_EMIT_ARM            (1)

//#define MICROPY_LONGINT_IMPL        (MICROPY_LONGINT_IMPL_MPZ)
//#define MICROPY_FLOAT_IMPL          (MICROPY_FLOAT_IMPL_FLOAT)

#define MICROPY_WARNINGS            (1)
#define MICROPY_ERROR_REPORTING                 (MICROPY_ERROR_REPORTING_TERSE)
#define MICROPY_INCLUDED_LIB_UTILS_INTERRUPT_CHAR_H (1)

#define MICROPY_USE_READLINE        (1)
#define MICROPY_USE_READLINE_HISTORY (1)

//#define MICROPY_STANDALONE (1)

#ifndef MICROPY_PY_BUILTINS_HELP_TEXT
#define MICROPY_PY_BUILTINS_HELP_TEXT nanopyos_help_text
#endif

#define MICROPY_PY_BUILTINS_INPUT (1)
#define MICROPY_PY_BUILTINS_POW3 (1)
#define MICROPY_PY_BUILTINS_FLOAT (1)
#define MICROPY_PY_FUNCTION_ATTRS (1)
#define MICROPY_PY_SYS_STDIO_BUFFER (1)
#define MICROPY_PY_SYS_STDFILES (0)
#define MICROPY_VFS                 (0)



#define MICROPY_PY_ASYNC_AWAIT                  (0)
#define MICROPY_PY_BUILTINS_SET                 (1)
#define MICROPY_PY_ATTRTUPLE                    (0)
#define MICROPY_PY_COLLECTIONS                  (0)
#define MICROPY_PY_MATH                         (1)
#define MICROPY_PY_IO                           (0)
#define MICROPY_PY_STRUCT                       (0)


// Type definitions for the specific machine
typedef int64_t mp_int_t;
typedef uint64_t mp_uint_t;
typedef long mp_off_t;

#define MP_STATE_PORT MP_STATE_VM
#define MICROPY_PORT_ROOT_POINTERS \
const char *readline_hist[8];
// Need to provide a declaration/definition of alloca()
#include <alloca.h>
