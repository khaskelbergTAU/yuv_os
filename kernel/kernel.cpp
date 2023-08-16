#include "kernel.h"

extern "C" int kernel_main(paging::PML4_entry page_table[512], uintptr_t kernel_start, uintptr_t kernel_end, uintptr_t kernel_virtual_base)
{
    using namespace serial;
    gdt::init_gdt();
    interrupts::init_interrupts();
    paging::PageAllocator mapper;
    screen.clear();
    screen.set_color(VGA_COLOR::WHITE, VGA_COLOR::BLACK);
    screen.writestr("Hello, World!\n");
    mapper.map_kernel(kernel_start, kernel_end, kernel_virtual_base);
    loop:
    goto loop;

    screen.writestr("is this thing working?\n");
    screen.writestr("Checking If port is valid...\n");
    if(DEBUG_PORT.valid())
    {
        screen.printf("Port seems to be valid.\nport number is %x\n", DEBUG_PORT.get_portnum());
    }
    else
    {
        screen.writestr("nope. port is broken.\n");
        return -1;
    }

    INFO("System Up!")
    DEBUG_PORT.printf("the kernel page table value is %x.:16\n", ((uint64_t *)page_table)[0x1ff]);
    DEBUG_PORT.printf("The kernel takes up addresses %p to %p to %p\n", kernel_start, kernel_end);
    return 0;
}
