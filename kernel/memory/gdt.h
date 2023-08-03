#ifndef _KERNEL_MEMORY_GDT_H
#define _KERNEL_MEMORY_GDT_H
#include <stdint.h>
#include "../utils/string.h"


namespace gdt
{
struct __attribute__((packed)) gdt_entry_access_t
{
    uint8_t accessed : 1;
    uint8_t read_write : 1;
    uint8_t direction_conforming : 1;
    uint8_t exec : 1;
    uint8_t desc_type : 1;
    uint8_t desc_priv_lvl : 2;
    uint8_t present : 1;
    gdt_entry_access_t(
                        uint8_t accessed,
                        uint8_t read_write,
                        uint8_t direction_conforming,
                        uint8_t exec,
                        uint8_t desc_type,
                        uint8_t desc_priv_lvl) :
                            accessed(accessed & 1 ),
                            read_write(read_write & 1),
                            direction_conforming(direction_conforming & 1),
                            exec(exec & 1),
                            desc_type(desc_type & 1),
                            desc_priv_lvl(desc_priv_lvl & 3),
                            present(1) {}
    gdt_entry_access_t() = default;
};
static_assert(sizeof(gdt_entry_access_t) == 1);
struct __attribute__((packed)) gdt_entry_t
{
    uint16_t limit_low : 16;
    uint32_t base_low : 24;
    gdt_entry_access_t access;
    uint8_t limit_high : 4;
    uint8_t res : 1;
    uint8_t long_mode : 1;
    uint8_t db : 1;
    uint8_t granularity : 1;
    uint8_t base_high : 8;

    public:
        gdt_entry_t(uint32_t base, uint32_t limit, uint8_t long_mode, uint8_t db, uint8_t granularity, gdt_entry_access_t access) : access(access),
                    long_mode(long_mode & 1),
                    db(db & 1),
                    granularity(granularity & 1)
        {
            base_low = base & ((1 << 24) - 1);
            base_high = (base >> 24) & ((1 << 8) - 1);
            limit_low = limit & ((1 << 16) - 1);
            limit_high = (limit >> 16) & ((1 << 4) - 1);
        }
        gdt_entry_t() = default;
        uint32_t base()
        {
            return base_low || (static_cast<uint32_t>(base_high) << 24);
        }
        uint32_t limit()
        {
            return limit_low || (static_cast<uint32_t>(limit_high) << 16);
        }
        void set_base(uint32_t base)
        {
            base_low = base & ((1 << 24) - 1);
            base_high = ((base >> 23) & ((1 << 8) - 1));
        }
};

static_assert(sizeof(gdt_entry_t) == 8);

extern gdt_entry_t GDT[64];

void init_gdt();
} // namespace gdt

extern "C"
{
    void _load_gdt(gdt::gdt_entry_t *GDT, uint16_t limit);
    void _reload_segments();
}
#endif/* _KERNEL_MEMORY_GDT_H */
