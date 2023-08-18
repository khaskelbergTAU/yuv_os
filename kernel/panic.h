#ifndef _KERNEL_PANIC_H
#define _KERNEL_PANIC_H
#include "utils/video.h"
#define YUV_ASSERT(arg, ...) do {if(!(arg)) {screen.printf(__VA_ARGS__); screen.printf("LINE %d at %s\n", __LINE__, __FILE__); kernel_panic("");}} while(0)
extern "C" {
    void kernel_panic(const char *s);
    void cause_interrupt();
}

#endif/* _KERNEL_PANIC_H */
