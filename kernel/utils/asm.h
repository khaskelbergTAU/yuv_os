#ifndef _KERNEL_UTILS_ASM_H
#define _KERNEL_UTILS_ASM_H

#include <stdint.h>
namespace inline_asm
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
#endif/* _KERNEL_UTILS_ASM_H */
