#include "video.h"
#include "utils/string.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static inline VGA_COMPOSED_COLOR vga_entry_color(VGA_COLOR fg, VGA_COLOR bg)
{
    return fg | (bg << 4);
}

static inline VGA_ENTRY vga_entry(unsigned char c, uint8_t color)
{
    return ((uint16_t)c) | (((uint16_t)color) << 8);
}

Video::Video()
{
    pos = 0;
    videomem = (uint16_t *)0xb8000;
    color = vga_entry_color(VGA_COLOR::BLACK, VGA_COLOR::BLACK);
    clear();
}

void Video::clear()
{
    VGA_ENTRY v = vga_entry(' ', vga_entry_color(VGA_COLOR::BLACK, VGA_COLOR::BLACK));
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            this->videomem[y * VGA_WIDTH + x] = v;
        }
    }
}

Video::~Video() {}
void Video::write_entry(VGA_ENTRY v, unsigned int x, unsigned int y)
{
    this->videomem[y * VGA_WIDTH + x] = v;
}

void Video::putc(char c)
{
    if (c == '\n')
        new_line();
    else
    {
        if (pos == VGA_WIDTH)
            new_line();
        write_entry(vga_entry(c, color), pos++, VGA_HEIGHT - 1);
    }
}

void Video::new_line()
{
    pos = 0;
    size_t y = 0;
    for (y = 0; y < VGA_HEIGHT - 1; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            this->videomem[y * VGA_WIDTH + x] = this->videomem[(y + 1) * VGA_WIDTH + x];
        }
    }
    VGA_ENTRY v = vga_entry(' ', vga_entry_color(VGA_COLOR::BLACK, VGA_COLOR::BLACK));
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
        this->videomem[y * VGA_WIDTH + x] = v;
    }
}

void Video::set_color(VGA_COLOR fg, VGA_COLOR bg)
{
    color = vga_entry_color(fg, bg);
}

void Video::write(const char *p, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        this->putc(p[i]);
    }
}

void Video::writestr(const char *p)
{
    write(p, strlen(p));
}
