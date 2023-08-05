#ifndef _KERNEL_PANIC_H
#define _KERNEL_PANIC_H
extern "C" {
    void kernel_panic();
    void cause_interrupt();
}

#endif/* _KERNEL_PANIC_H */
