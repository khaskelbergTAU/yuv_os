#ifndef _KERNEL_MEMORY_PAGING_H
#define _KERNEL_MEMORY_PAGING_H
#include <stdint.h>
#include "utils/video.h"

namespace paging
{
struct __attribute__((__packed__)) PML4_entry
{
    uint8_t present : 1;      // bit 0: always 1
    uint8_t rw : 1;           // bit 1: read/write
    uint8_t us : 1;           // bit 2: user or supervisor
    uint8_t pwt : 1;          // bit 3: page-level write-through
    uint8_t pcd : 1;          // bit 4: page-level cache disable
    uint8_t a : 1;            // bit 5: accessed
    uint8_t ignored : 1;      // bit 6
    uint8_t rsvd : 1;           // bit 7: reserved
    uint8_t ignored2 : 4;     // bit 8 - 11
    uint64_t page_table : 36; // bit 12 - 47: physical address of 4KB aligned page table referenced by this entry
    uint16_t ignored3 : 15; // bits 48 - 62
    uint8_t xd : 1; // bit 63 - execute disable
    PML4_entry(uint8_t rw, uint8_t us, uint8_t pwt,  uint8_t pcd, uint64_t pdpt_addr, uint8_t xd) :
                present(1),
                rw(rw),
                us(us),
                pwt(pwt),
                pcd(pcd),
                a(0),
                ignored(0),
                rsvd(0),
                ignored2(0),
                page_table(pdpt_addr >> 12),
                ignored3(0),
                xd(xd)
                {}
};
static_assert(sizeof(PML4_entry) == 8);
struct __attribute__((__packed__)) PDPT_entry
{
    uint8_t present : 1;      // bit 0: always 1
    uint8_t rw : 1;           // bit 1: read/write
    uint8_t us : 1;           // bit 2: user or supervisor
    uint8_t pwt : 1;          // bit 3: page-level write-through
    uint8_t pcd : 1;          // bit 4: page-level cache disable
    uint8_t a : 1;            // bit 5: accessed
    uint8_t ignored : 1;      // bit 6
    uint8_t ps : 1;           // bit 7: page size (huge page)
    uint8_t ignored2 : 4;     // bit 8 - 11
    uint64_t page_table : 36; // bit 12 - 47: physical address of 4KB aligned page table referenced by this entry
    uint16_t ignored3 : 15; // bits 48 - 62
    uint8_t xd : 1; // bit 63 - execute disable
};
static_assert(sizeof(PDPT_entry) == 8);

using PD_entry = PDPT_entry;
struct __attribute__((__packed__)) PT_entry
{
    uint8_t present : 1;      // bit 0: always 1
    uint8_t rw : 1;           // bit 1: read/write
    uint8_t us : 1;           // bit 2: user or supervisor
    uint8_t pwt : 1;          // bit 3: page-level write-through
    uint8_t pcd : 1;          // bit 4: page-level cache disable
    uint8_t a : 1;            // bit 5: accessed
    uint8_t d : 1;            // bit 6: dirty
    uint8_t pat : 1;          // bit 7: must be 0 unless PAT supported
    uint8_t g : 1;            // bit 8: global translation
    uint8_t ignored2 : 3;     // bit 9 - 11
    uint64_t page_frame : 36; // bit 12 - 47: physical address of 4KB aligned page frame referenced by this entry
    uint16_t ignored3 : 15; // bits 48 - 62
    uint8_t xd : 1; // bit 63 - execute disable
};
static_assert(sizeof(PT_entry) == 8);

void map_kernel(uintptr_t kernel_start, uintptr_t kernel_end, PML4_entry old_pml4[512]);

void _load_cr3(PML4_entry pml4[512]);

}

#endif/* _KERNEL_MEMORY_PAGING_H */
