#include "video.h"
#include "serial.h"
#include "paging.h"
#include "utils/debug.h"

serial::SerialPort DEBUG_PORT{0x3f8};

extern "C" void kernel_main(directory page_table[1024])
{
    Video v;
    v.clear();
    v.set_color(VGA_COLOR::WHITE, VGA_COLOR::BLACK);
    v.writestr("Hello, World!\n");

    INFO("System Up!")
    DEBUG_PORT.printf("the first value is %b:32\n", ((uint32_t *)page_table)[0xc0000000 >> 22]);
}
