#include "kernel.h"

char *_kernel_start = &__kernel_start;
char *_kernel_end = &__kernel_end;

extern "C" int kernel_main(directory page_table[1024])
{
    using namespace serial;
    gdt::init_gdt();
    interrupts::init_interrupts();
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

    INFO("System Up!")
    DEBUG_PORT.printf("the kernel page table value is %b.:32\n", ((uint32_t *)page_table)[0xc0000000 >> 22]);
    DEBUG_PORT.printf("The kernel takes up addresses %p to %p\n", _kernel_start, _kernel_end);
    return 0;
}
