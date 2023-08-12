#include "kernel_mapper.h"

char *_kernel_start = &__kernel_start;
char *_kernel_end = &__kernel_end;

extern "C" void __map_kernel()
{
    uintptr_t range = reinterpret_cast<uintptr_t>(_kernel_end);
    range = range + 1;
}

void map_addr(uintptr_t v_addr, uintptr_t p_addr)
{
    v_addr = v_addr + p_addr;
    
}