#ifndef _KERNEL_PANIC_H
#define _KERNEL_PANIC_H
#include "utils/video.h"
extern "C" {
    void kernel_panic(const char *s);
    void cause_interrupt();
}

#endif/* _KERNEL_PANIC_H */
