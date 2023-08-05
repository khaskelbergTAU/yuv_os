#include "panic.h"
extern "C"
{
    void kernel_panic()
    {
        loop:
            goto loop;
    }
}