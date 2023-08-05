#ifndef _KERNEL_UTILS_VIDEO_H
#define _KERNEL_UTILS_VIDEO_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "utils/string.h"
#include "printf.h"

using VGA_ENTRY = uint16_t;
using VGA_COMPOSED_COLOR = uint8_t;
enum VGA_COLOR
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
};

class Video
{
public:
    Video(uint16_t *videomem);
    void clear();
    void set_color(VGA_COLOR fg, VGA_COLOR bg);
    void write(const char *p, size_t len);
    void putc(char c);
    void writestr(const char *s);
    void new_line();
    void del_line();
    void printf(const char *fmt, ...);
    void set_cursor(bool on);

private:
    static const size_t VGA_WIDTH = 80;
    static const size_t VGA_HEIGHT = 25;
    void write_entry(VGA_ENTRY e, unsigned int x, unsigned int y);
    VGA_ENTRY *videomem;
    VGA_COMPOSED_COLOR color;
    size_t pos_head{};
    uint32_t pos[VGA_HEIGHT]{};
};

extern Video screen;

#endif/* _KERNEL_UTILS_VIDEO_H */
