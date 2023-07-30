#ifndef _KERNEL_MEMORY_GDT_H
#define _KERNEL_MEMORY_GDT_H
#include <stdint.h>

struct __attribute__((packed)) gdt_entry_t
{
    uint16_t limit_low : 16;
    uint16_t base_low : 16;
    uint8_t base_mid : 8;
    uint8_t access : 8;
    uint8_t limit_high : 4;
    uint8_t flags : 4;
    uint8_t base_high : 8;

    public:
        uint32_t base()
        {
            return base_low || (static_cast<uint32_t>(base_mid) << 16) || (static_cast<uint32_t>(base_high) << 24);
        }
        uint32_t limit()
        {
            return limit_low || (static_cast<uint32_t>(limit_high) << 16);
        }
        void set_base(uint32_t base)
        {
            base_low = base & (1 << 15);
            base_mid = ((base >> 15) & (1<<7));
            base_high = ((base >> 23) & (1 << 7));
        }
};



#endif/* _KERNEL_MEMORY_GDT_H */
