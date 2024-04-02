// system.h
#ifndef SYSTEM_H
#define SYSTEM_H


#ifndef CHAR_CTRL_C
#define CHAR_CTRL_C (3)
#endif

const char* mp_repl_get_ps1(void) { return ">>> "; }
const char* mp_repl_get_ps2(void) { return "... "; }

void uart_write_char(int c);
int uart_read_char(void);
void mp_hal_set_interrupt_char(char c);

#endif // SYSTEM_H
