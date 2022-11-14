#include "video.h"
#include "serial.h"

serial::SerialPort DEBUG_PORT{0x3f8};

extern "C" void kernel_main()
{
    Video v;
    v.clear();
    v.set_color(VGA_COLOR::WHITE, VGA_COLOR::BLACK);
    v.writestr("Hello, World!\n");
    DEBUG_PORT.printf("\nSystem Up!, haha funny %d number and in hex %x\n", -2559891, 0xffabed9);
    DEBUG_PORT.writestr("after printf\n");
}
