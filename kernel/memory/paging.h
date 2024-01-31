#ifndef _KERNEL_MEMORY_PAGING_H
#define _KERNEL_MEMORY_PAGING_H
#include <stdint.h>
#include "utils/video.h"
#include "utils/serial.h"
#include "utils/string.h"
#include "utils/datastructs/double_linked_list.h"
#include "utils/datastructs/dynamic_bitset.h"
#include "page_list.h"
#include "utils/singleton.h"
#include "dumb_allocator.h"
#include "mem.h"

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
#define PAGE_MAX_ORDER 12
#define PAGE_ROUND_UP(x) ROUND_UP(x, PAGE_SIZE)

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
    PML4_entry(uint8_t rw, uint8_t us, uint8_t pwt, uint8_t pcd, uint64_t pdpt_addr, uint8_t xd) :
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
    PDPT_entry(uint8_t rw, uint8_t us, uint8_t pwt, uint8_t pcd, uint8_t ps, uint64_t map_addr, uint8_t xd) :
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
    PD_entry(uint8_t rw, uint8_t us, uint8_t pwt, uint8_t pcd, uint8_t ps, uint64_t map_addr, uint8_t xd) :
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
    PT_entry(uint8_t rw, uint8_t us, uint8_t pwt, uint8_t pcd, uint64_t map_addr, uint8_t xd) :
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

/*
 * i realised that this allocator is way too complicated for my needs
 * im leaving what i implemented here in case it becomes useful
 * for now, ill just use the simple bit set allocator.
 */
template<size_t N>
class BuddyAllocator
{
    std::dynamic_bitset<DumbAllocator> m_sets[N];
    uint64_t m_page_count = 0;
    PageList m_page_lists[N];
    uintptr_t m_mem_start = 0;
    public:
    BuddyAllocator() = default;
    BuddyAllocator(uint64_t page_count, uintptr_t kernel_base)
        : m_page_count(page_count), m_mem_start(kernel_base)
    {
        for (size_t i = 0; i < N; i++)
        {
            new (&m_sets[i]) std::dynamic_bitset<DumbAllocator>(page_count >> i);
            m_sets[i].clear();
        }
        m_mem_start = PAGE_ROUND_UP(DumbAllocator::getInstance()->mem_end());
        m_page_count -= m_mem_start >> 12;
    }
    void* alloc(size_t order)
    {
        YUV_ASSERT(order < N, "requested order %u too large, max order %u\n", order, N);
        PageList& lst = m_page_lists[order];
        auto& set = m_sets[order];
        void* retval = nullptr;
        if (!lst.empty())
        {
            Page* page = lst.pop();
            set[reinterpret_cast<uintptr_t>(page) >> (order + 12)].set();
            return (void *)page;
        }
        if (set.has_zero())
        {
            uint64_t indx = set.first_clear();
            uint64_t higher_order_indx = indx;
            for(size_t curr_order = order; curr_order < N; curr_order++)
            {
                m_sets[curr_order][higher_order_indx].set();
                higher_order_indx /= 2;
            }
            retval = reinterpret_cast<void*>(m_mem_start + (indx << (order + 12)));
        }
        return retval;
    }
};

class BitSetPageAllocator
{
private:
    std::dynamic_bitset<DumbAllocator> pages;

public:
    BitSetPageAllocator() {}
    BitSetPageAllocator(uint64_t pagecnt) : pages(pagecnt) {}
    uint64_t alloc()
    {
        uint64_t indx = pages.first_clear();
        pages[indx].set();
        return indx << 12;
    }
    void free(uint64_t pageaddr)
    {
        pages[pageaddr >> 12].clear();
    }
};

class PageAllocator : public UnsafeSingleton<PageAllocator>
{
    private:
    uintptr_t m_kernel_base;
    uint64_t m_page_count;
    BitSetPageAllocator m_allocator;
    template<class T>
    static bool present(T entry) { return entry.present != 0; }

    public:
    static void paging_init(uintptr_t kernel_base, uintptr_t kernel_end, uint64_t page_count);
    static uintptr_t V2P(uintptr_t vaddr)
    {
        return vaddr - getInstance()->m_kernel_base;
    }

    static uintptr_t P2V(uintptr_t paddr)
    {
        return paddr + getInstance()->m_kernel_base;
    }

    static uint64_t page_count()
    {
        return getInstance()->m_page_count;
    }

    static void *alloc_page()
    {
        return (void *)(getInstance()->m_kernel_base + getInstance()->m_allocator.alloc());
    }
    static void free_page(void *pageaddr)
    {
        getInstance()->m_allocator.free((uint64_t)pageaddr - getInstance()->m_kernel_base);
    }
};
void _load_cr3(PML4_entry pml4[512]);

}

#endif/* _KERNEL_MEMORY_PAGING_H */