#include <errno.h>

static int local_errno;

int *__errno(void) {
    return &local_errno;
}
