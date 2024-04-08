static inline mp_uint_t mp_hal_ticks_ms(void) {
    return 0;
}
//

static inline void mp_hal_set_interrupt_char(char c) {}
#define mp_hal_pin_obj_t

//#if MICROPY_PY_BUILTINS_INPUT && MICROPY_USE_READLINE == 0
//
//#include <malloc.h>
//#include "py/misc.h"
//#include "input.h"
//#define mp_hal_readline mp_hal_readline
//static inline int mp_hal_readline(vstr_t *vstr, const char *p) {
//    char *line = prompt((char *)p);
//    vstr_add_str(vstr, line);
//    free(line);
//    return 0;
//}
//
//#elif MICROPY_PY_BUILTINS_INPUT && MICROPY_USE_READLINE == 1
//
//#include "py/misc.h"
//#include "shared/readline/readline.h"
//void mp_hal_stdio_mode_raw(void);
//void mp_hal_stdio_mode_orig(void);
//// For built-in input() we need to wrap the standard readline() to enable raw mode
//#define mp_hal_readline mp_hal_readline
//static inline int mp_hal_readline(vstr_t *vstr, const char *p) {
//    mp_hal_stdio_mode_raw();
//    int ret = readline(vstr, p);
//    mp_hal_stdio_mode_orig();
//    return ret;
//}
//
//#endif