#ifndef _KERNEL_UTILS_IO_ASM_H
#define _KERNEL_UTILS_IO_ASM_H

#include <stdint.h>
namespace io
{
    inline void outb(uint16_t portnum, uint8_t val)
    {
        asm volatile(
            "outb %1, %0" ::"Nd"(portnum), "a"(val));
    }
    inline uint8_t inb(uint16_t portnum)
    {
        uint8_t c;
        asm volatile(
            "inb %1, %0"
            : "=a"(c)
            : "Nd"(portnum));
        return c;
    }
}
#endif