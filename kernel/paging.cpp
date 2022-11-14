#include "paging.h"

extern "C" void map_kernel(directory level4table[1024])
{
    directory kernel_page_1 = {};
    directory kernel_page_2 = {};

    int kernel_virtual_base = 0xc0000000;
    int kernel_page_number = kernel_virtual_base >> 22;

    kernel_page_1.present = 1;
    kernel_page_1.rw = 1;
    level4table[0] = kernel_page_1;
    level4table[1024 + 2048] = kernel_page_2;

    _load_cr3(level4table);
    _enable_paging();
}