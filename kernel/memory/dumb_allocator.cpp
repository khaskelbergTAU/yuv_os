#include "dumb_allocator.h"

void DumbAllocator::init(uintptr_t mem_end)
{
    m_mem_end = mem_end;
}
void *DumbAllocator::alloc(size_t alloc_size)
{
    void *retval = reinterpret_cast<void *>(m_mem_end);
    m_mem_end += alloc_size;
    return retval;
}