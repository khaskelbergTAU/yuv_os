#include "paging.h"
namespace paging
{
void PageAllocator::paging_init(uintptr_t kernel_base, uintptr_t kernel_end, uint64_t page_count)
{
    PageAllocator &allocator = getInstance();
    allocator.m_kernel_base = kernel_base;
    allocator.m_page_count = page_count;
    uint64_t* mem_areas[PAGE_MAX_ORDER];
    for (size_t i = 0; i < PAGE_MAX_ORDER; i++)
    {
        size_t bitset_alloc_size = ROUND_UP(page_count >> (i + 4), 8);
        std::memset(reinterpret_cast<void *>(kernel_end), 0, bitset_alloc_size);
        mem_areas[i] = reinterpret_cast<uint64_t*>(kernel_end);
        kernel_end += bitset_alloc_size;
    }
    BuddyAllocator<PAGE_MAX_ORDER> tmp(page_count, mem_areas);
    allocator.m_allocator = tmp;
}
void _load_cr3(PML4_entry pml4[512])
{
    asm("mov %0, %%cr3" :: "r" (pml4));
}
}