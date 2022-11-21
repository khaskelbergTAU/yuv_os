#ifndef VIDEO_H
#define VIDEO_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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
    ~Video();
    void clear();
    void set_color(VGA_COLOR fg, VGA_COLOR bg);
    void write(const char *p, size_t len);
    void putc(char c);
    void writestr(const char *s);
    void new_line();

private:
    void write_entry(VGA_ENTRY e, unsigned int x, unsigned int y);
    VGA_ENTRY *videomem;
    VGA_COMPOSED_COLOR color;
    unsigned int pos;
};

#endif
