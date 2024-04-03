// empty file
//extern int mp_interrupt_char;
static inline mp_uint_t mp_hal_ticks_ms(void) {
    return 0;
}

void mp_hal_set_interrupt_char(int c);
//static inline void mp_hal_set_interrupt_char(char c) {}
#define mp_hal_pin_obj_t