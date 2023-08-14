#include "panic.h"
extern "C"
{
    void kernel_panic(const char *s)
    {
        screen.printf("kernel panic!! %s", s);
        loop:
            goto loop;
    }
}