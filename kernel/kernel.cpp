#include "kernel.h"
extern "C"
{
    char __kernel_start;
    char __kernel_end;
}

char *_kernel_start = &__kernel_start;
char *_kernel_end = &__kernel_end;

serial::SerialPort DEBUG_PORT{0x3f8};

extern "C" int kernel_main(directory page_table[1024])
{
    Video v{reinterpret_cast<uint16_t *>(0xc00b8000)};
    v.clear();
    v.set_color(VGA_COLOR::WHITE, VGA_COLOR::BLACK);
    v.writestr("Hello, World!\n");

    v.writestr("is this thing working?\n");
    v.writestr("Checking If port is valid...\n");
    if(DEBUG_PORT.valid())
    {
        v.writestr("Port seems to be valid.\n");
    }
    else
    {
        v.writestr("nope. port is broken.\n");
        return -1;
    }
    INFO("System Up!")
    DEBUG_PORT.printf("the kernel page table value is %b:32\n", ((uint32_t *)page_table)[0xc0000000 >> 22]);
    DEBUG_PORT.printf("The kernel takes up addresses %p to %p\n", _kernel_start, _kernel_end);
    DEBUG_PORT.printf("the first value is %b:32\n", (reinterpret_cast<uint32_t *>(page_table))[0xc0000000 >> 22]);
    return 0;
}
