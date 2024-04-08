// system.h
#ifndef SYSTEM_H
#define SYSTEM_H


#ifndef CHAR_CTRL_C
#define CHAR_CTRL_C (3)
#endif

void mp_hal_stdout_tx_strn(const char *, long unsigned int);

int mp_hal_stdin_rx_chr(void);


void printc(const char *s);

void printc_hex(unsigned long value);

void uart_init();

#endif // SYSTEM_H
