#ifndef _KERNEL_BOOT_KERNEL_MAPPER_H
#define _KERNEL_BOOT_KERNEL_MAPPER_H
#include <stdint.h>

extern "C"
{
    char __kernel_start;
    char __kernel_end;
    void __map_kernel() __attribute__((section("kernel_mapper")));
}

#endif/* _KERNEL_BOOT_KERNEL_MAPPER_H */
