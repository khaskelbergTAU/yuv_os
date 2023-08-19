#include "kernel.h"

extern "C" int kernel_main(paging::PML4_entry page_table[512], uintptr_t kernel_start, uintptr_t kernel_end, uintptr_t kernel_virtual_base, multiboot_info *multiboot2)
{
    using namespace serial;

    gdt::init_gdt();
    interrupts::init_interrupts();
    boot_info info(multiboot2);
    screen.clear();
    screen.set_color(VGA_COLOR::WHITE, VGA_COLOR::BLACK);
    screen.writestr("Hello, World!\n");

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

    paging::PageAllocator::paging_init(kernel_virtual_base,
                                        kernel_end,
                                        (info.meminfo.mem_upper >> 2) + 0x120);
    INFO("System Up!")
    DEBUG_PORT.printf("the kernel page table value is %x.:16\n", ((uint64_t *)page_table)[0x1ff]);
    DEBUG_PORT.printf("The kernel takes up addresses %p to %p\n", kernel_start, DumbAllocator::getInstance()->mem_end());
    DEBUG_PORT.printf("We have %x memory available.\n", paging::PageAllocator::page_count() * 0x1000);
    void *ptr = paging::PageAllocator::alloc_page(0);
    DEBUG_PORT.printf("the page we got was allocated at %p\n", ptr);

    return 0;
}
