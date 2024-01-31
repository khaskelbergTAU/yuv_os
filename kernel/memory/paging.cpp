#include "paging.h"
namespace paging
{
void PageAllocator::paging_init(uintptr_t kernel_base, uintptr_t kernel_end, uint64_t page_count)
{
    DumbAllocator::getInstance()->init(kernel_end);
    PageAllocator *allocator = getInstance();
    allocator->m_kernel_base = kernel_base;
    allocator->m_page_count = page_count;
    new (reinterpret_cast<void *>(&allocator->m_allocator)) BitSetPageAllocator(page_count);
}
void _load_cr3(PML4_entry pml4[512])
{
    asm("mov %0, %%cr3" :: "r" (pml4));
}
}