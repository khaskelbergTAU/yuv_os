#include "paging.h"
namespace paging
{
    
void map_kernel(uintptr_t kernel_start, uintptr_t kernel_end, PML4_entry old_pml4[512])
{
    screen.printf("The kernel goes from %x to %x and the pt is at address %x.\n", kernel_start, kernel_end, old_pml4);
    while(kernel_end > kernel_start)
    {
        kernel_end--;
    }

}

void _load_cr3(PML4_entry pml4[512])
{
    asm("mov %%cr3, %0" :: "r" (pml4));
}
}