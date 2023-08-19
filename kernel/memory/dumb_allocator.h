#ifndef _KERNEL_MEMORY_DUMB_ALLOCATOR_H
#define _KERNEL_MEMORY_DUMB_ALLOCATOR_H
#include <stdint.h>
#include <stddef.h>
#include "utils/singleton.h"
class DumbAllocator : public UnsafeSingleton<DumbAllocator>
{
    uintptr_t m_mem_end;
    public:
    void init(uintptr_t mem_end);
    void *alloc(size_t alloc_size);
    void free(){};
    uintptr_t mem_end() {return m_mem_end;}

};

#endif/* _KERNEL_MEMORY_DUMB_ALLOCATOR_H */
