#include "video.h"
#include "serial.h"
#include "paging.h"

serial::SerialPort DEBUG_PORT{0x3f8};

extern "C" void kernel_main(directory page_table[1024])
{
    Video v;
    v.clear();
    v.set_color(VGA_COLOR::WHITE, VGA_COLOR::BLACK);
    v.writestr("Hello, World!\n");
    DEBUG_PORT.printf("\nSystem Up!\naddress of the kevel 4 table is %p\n", page_table);
    DEBUG_PORT.printf("the first value is %b:32\n", ((uint32_t *)page_table)[0xc0000000 >> 22]);
}
