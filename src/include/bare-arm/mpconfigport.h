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

#define MICROPY_BANNER_MACHINE "NanoPyOS"
#define MICROPY_HW_BOARD_NAME "minimal"
#define MICROPY_HW_MCU_NAME "unknown-cpu"


#define MP_STATE_PORT MP_STATE_VM

// Enable most common features
#define MICROPY_CONFIG_ROM_LEVEL_BASIC_FEATURES (20)

// Compiler configuration
#define MICROPY_ENABLE_COMPILER                 (1)


// Ensure builtinimport.c works with -m.
#define MICROPY_MODULE_OVERRIDE_MAIN_IMPORT (1)


#define MICROPY_ENABLE_EXTERNAL_IMPORT (1)
#define MICROPY_ALLOC_PATH_MAX            (512)

// Python internal features
#define MICROPY_MALLOC_USES_ALLOCATED_SIZE (1)
#define MICROPY_PY_EXEC             (1)
#define MICROPY_MEM_STATS           (1)
#define MICROPY_ENABLE_GC           (1)
#define MICROPY_PY_GC               (0)


#define MICROPY_MIN_USE_STDOUT (0)
#define MICROPY_HEAP_SIZE (256 * 1024 ) // 256 kilobytes


#define MICROPY_REPL_INFO           (1)

#define MICROPY_KBD_EXCEPTION       (1)
#define MICROPY_HELPER_REPL         (1)

#define MICROPY_NLR_AARCH64 (1)
//#define MICROPY_EMIT_ARM            (1)

#define MICROPY_PY_SYS_PLATFORM     "qemu-arm"


//#define MICROPY_LONGINT_IMPL        (MICROPY_LONGINT_IMPL_MPZ)
//#define MICROPY_FLOAT_IMPL          (MICROPY_FLOAT_IMPL_FLOAT)

#define MICROPY_WARNINGS            (1)
#define MICROPY_ERROR_REPORTING                 (MICROPY_ERROR_REPORTING_TERSE)
#define MICROPY_INCLUDED_LIB_UTILS_INTERRUPT_CHAR_H (1)


#define MICROPY_REPL_EVENT_DRIVEN (1)


#define MICROPY_USE_READLINE        (0)
#define MICROPY_USE_READLINE_HISTORY (0)

#define MICROPY_PY_BUILTINS_COMPILE (1)
#define MICROPY_PY_BUILTINS_EXECFILE (1)
#define MICROPY_PY_BUILTINS_INPUT (1)
#define MICROPY_PY_BUILTINS_POW3 (1)
#define MICROPY_PY_BUILTINS_FLOAT (1)
#define MICROPY_PY_FUNCTION_ATTRS (1)
#define MICROPY_PY_SYS_STDIO_BUFFER (1)
#define MICROPY_PY_SYS_STDFILES (0)
#define MICROPY_PY_BUILTINS_HELP    (1)
#define MICROPY_PY_BUILTINS_HELP_MODULES (1)
#define MICROPY_PY_BUILTINS_HELP_TEXT nanopyos_help_text
#define MICROPY_VFS                 (0)

#define MICROPY_PY_ASYNC_AWAIT                  (0)
#define MICROPY_PY_BUILTINS_SET                 (1)
#define MICROPY_PY_ATTRTUPLE                    (1)
#define MICROPY_PY_COLLECTIONS                  (1)
#define MICROPY_PY_MATH                         (1)
#define MICROPY_PY_IO                           (0)
#define MICROPY_PY_STRUCT                       (1)
//#define MICROPY_PY_SYS_EXECUTABLE (1)

// Disable stackless by default.
#ifndef MICROPY_STACKLESS
#define MICROPY_STACKLESS           (0)
#define MICROPY_STACKLESS_STRICT    (0)
#endif
#define MICROPY_GC_STACK_ENTRY_TYPE uint64_t


#define MICROPY_PORT_ROOT_POINTERS \
    const char *readline_hist[8]; \
    \
    mp_obj_t pyb_hid_report_desc; \
    \
    mp_obj_t pyb_config_main; \
    \
    mp_obj_t pyb_switch_callback; \
    \
    mp_obj_t pin_class_mapper; \
    mp_obj_t pin_class_map_dict; \
    \
    mp_obj_t pyb_extint_callback[PYB_EXTI_NUM_VECTORS]; \
    \
    struct _soft_timer_entry_t *soft_timer_heap;

// Need to provide a declaration/definition of alloca()
#include <alloca.h>


// For debugging purposes, make printf() available to any source file.
#include <stdio.h>
