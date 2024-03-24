#include <stdarg.h>
#include <stdint.h>


const char *text;
volatile uint8_t *uart = (uint8_t *) 0x09000000;

extern int main(void) __attribute__((visibility("default")));


void putchar(char c) { *uart = c; }


void print(const char *s) {
    while (*s != '\0') {
        putchar(*s);
        s++;
    }
}

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    char c;
    while (*fmt) {
        if ((c = *fmt++) != '%') {
            putchar(c);
            continue;
        }
        switch ((c = *fmt++)) {
            case 's':
                print(va_arg(ap,
                const char *));
                break;
            default:
                putchar(c);
        }
    }
    va_end(ap);
    return 0;
}

int main(void) {
    print("Result: ");
    printf("from %s", "printf");
    print("\n");

    while (1) {}
    return 0;

}

// Called if an exception is raised outside all C exception-catching handlers.
void nlr_jump_fail(void *val) {
    for (;;) {
    }
}
