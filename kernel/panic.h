#ifndef _KERNEL_PANIC_H
#define _KERNEL_PANIC_H
extern "C" {
    void kernel_panic();
    void divide_by_zero();
}

#endif/* _KERNEL_PANIC_H */
