#include "video.h"
#include "serial.h"

serial::SerialPort DEBUG_PORT{0x3f8};

extern "C" void kernel_main()
{
    Video v;
    v.clear();
    v.set_color(VGA_COLOR::WHITE, VGA_COLOR::BLACK);
    v.writestr("Hello, World!\n");
    char ptr[4];
    const char* str = "str";
    for(int i=0;i<4;i++){
        ptr[i] = str[i];
    }
    DEBUG_PORT.printf("\nSystem Up!, haha funny %d number and in hex %x:8\n", -2559891, (int)ptr);
    DEBUG_PORT.printf("the string is %s haha\n", "a string!");
    DEBUG_PORT.writestr("after printf\n");
}
