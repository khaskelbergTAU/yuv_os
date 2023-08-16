#ifndef _KERNEL_MEMORY_PAGING_H
#define _KERNEL_MEMORY_PAGING_H
#include <stdint.h>
#include "utils/video.h"
#include "utils/serial.h"
#include "utils/string.h"

#define TABLE_INDEX(addr, off) ((addr >> off) & 0x1ff)
#define PML4_INDX(addr) TABLE_INDEX(addr, 39)
#define PDPT_INDX(addr) TABLE_INDEX(addr, 30)
#define PD_INDX(addr) TABLE_INDEX(addr, 21)
#define PT_INDX(addr) TABLE_INDEX(addr, 12)
#define PAGE_SIZE 0x1000ULL
#define NUM_ENTRIES 512ULL
#define PD_ENTRY_MAP_SIZE (PAGE_SIZE * NUM_ENTRIES)
#define PDPT_ENTRY_MAP_SIZE (PD_ENTRY_MAP_SIZE * NUM_ENTRIES)
#define PML4_ENTRY_MAP_SIZE (PDPT_ENTRY_MAP_SIZE * NUM_ENTRIES)
#define PZ(addr) do {std::memset(addr, 0, PAGE_SIZE);} while(0)

namespace paging
{
using Page = uint8_t[PAGE_SIZE];
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
    PDPT_entry(uint8_t rw, uint8_t us, uint8_t pwt,  uint8_t pcd, uint8_t ps, uint64_t map_addr, uint8_t xd) :
                present(1),
                rw(rw),
                us(us),
                pwt(pwt),
                pcd(pcd),
                a(0),
                ignored(0),
                ps(ps),
                ignored2(0),
                page_table(map_addr >> 12),
                ignored3(0),
                xd(xd)
                {}
};
static_assert(sizeof(PDPT_entry) == 8);

struct __attribute__((__packed__)) PD_entry
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
    PD_entry(uint8_t rw, uint8_t us, uint8_t pwt,  uint8_t pcd, uint8_t ps, uint64_t map_addr, uint8_t xd) :
                present(1),
                rw(rw),
                us(us),
                pwt(pwt),
                pcd(pcd),
                a(0),
                ignored(0),
                ps(ps),
                ignored2(0),
                page_table(map_addr >> 12),
                ignored3(0),
                xd(xd)
                {}
};
static_assert(sizeof(PD_entry) == 8);
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
    PT_entry(uint8_t rw, uint8_t us, uint8_t pwt,  uint8_t pcd, uint64_t map_addr, uint8_t xd) :
                present(1),
                rw(rw),
                us(us),
                pwt(pwt),
                pcd(pcd),
                a(0),
                d(0),
                pat(0),
                g(0),
                ignored2(0),
                page_frame(map_addr >> 12),
                ignored3(0),
                xd(xd)
                {}
};
static_assert(sizeof(PT_entry) == 8);
class PageAllocator
{
private:
    Page *Page_Pool; // a pool of 3 page tables, used for temporary mappings when needed.
    // these are all variables to track where the kernel pages start and end.
    // PDPT points at the start of all PDPTs, and so on.
    PML4_entry *PML4;
    PDPT_entry *PDPT;
    PD_entry *PD;
    PT_entry *PT;
    uintptr_t FRAME;
    uintptr_t alloc_end;
    template<class T>
    static bool present(T entry) {return entry.present != 0;}
    PDPT_entry *get_pml4_entry(PML4_entry *pml4, uintptr_t vaddr);
    PD_entry *get_pdpt_entry(PDPT_entry *pdpt, uintptr_t vaddr);
    PT_entry *get_pd_entry(PD_entry *pd, uintptr_t vaddr);
    bool map_pt_entry(PT_entry *pt, uintptr_t vaddr, uintptr_t paddr);
    void map_addr(uintptr_t vaddr, uintptr_t paddr);
    uintptr_t tmp_map(uintptr_t paddr);
    static PDPT_entry *extract_addr(PML4_entry);
    static PD_entry *extract_addr(PDPT_entry);
    static PT_entry *extract_addr(PD_entry);
    static Page *extract_addr(PT_entry);
public:
    void map_kernel(uintptr_t kernel_start, uintptr_t kernel_end, uintptr_t kernel_base);
    void map(uint8_t *p_addr);
};

void _load_cr3(PML4_entry pml4[512]);

}

#endif/* _KERNEL_MEMORY_PAGING_H */
