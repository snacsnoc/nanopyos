// system.h
#ifndef SYSTEM_H
#define SYSTEM_H


#ifndef CHAR_CTRL_C
#define CHAR_CTRL_C (3)
#endif

//const char* mp_repl_get_ps1(void) { return ">>> "; }
//const char* mp_repl_get_ps2(void) { return "... "; }

void uart_write_char(int c);
int uart_read_char(void);

int uart_has_data(void);

//void mp_hal_stdout_tx_strn(const char *, long unsigned int);

//mp_uint_t mp_hal_stdin_rx_chr(void);
int mp_hal_stdin_rx_chr(void);
void mp_hal_stdout_tx_strn(const char *);


void uart_putc(char c);
void uart_putch(char c);

void pputchar(char c);

void print(const char *s);
void printc(const char *s);
void printc_hex(unsigned long value);
//void mp_hal_set_interrupt_char(char c);

#endif // SYSTEM_H
