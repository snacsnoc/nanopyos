#include <stdint.h>
#include <string.h>
#include "system.h"


extern uint64_t _estack, _sidata, _sdata, _edata, _sbss, _ebss, _start;

int main(void);


// Very simple ARM vector table.
const uint64_t isr_vector[] __attribute__((section(".isr_vector"))) = {
        (uint64_t) & _estack,
        (uint64_t) & _start,
};

