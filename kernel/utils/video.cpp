#include "video.h"

#define POS_CALC_INDX(ind) (((ind) + (pos_head)) % POS_SIZE)
#define POS_INDX(ind) (pos[POS_CALC_INDX(ind)])
#define POS_SIZE (sizeof(pos) / sizeof(pos[0]))

static inline VGA_COMPOSED_COLOR vga_entry_color(VGA_COLOR fg, VGA_COLOR bg)
{
    return fg | (bg << 4);
}

static inline VGA_ENTRY vga_entry(unsigned char c, uint8_t color)
{
    return ((uint16_t)c) | (((uint16_t)color) << 8);
}

Video::Video(uint16_t *videomem) : videomem(static_cast<VGA_ENTRY *>(videomem))
{
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
    memset(pos, 0, sizeof(pos));
}

void Video::write_entry(VGA_ENTRY v, unsigned int x, unsigned int y)
{
    this->videomem[y * VGA_WIDTH + x] = v;
}

void Video::putc(char c)
{
    switch (c)
    {
    case '\n':
    {
        new_line();
        break;
    }
    case '\b':
    {
        if (POS_INDX(VGA_HEIGHT - 1) > 0 and POS_INDX(VGA_HEIGHT - 1) < POS_SIZE - 1)
        {
            write_entry(vga_entry(0, VGA_COLOR::BLACK), --POS_INDX(VGA_HEIGHT - 1), VGA_HEIGHT - 1);
        }
        else
        {
            del_line();
        }
        break;
    }
    case '\t':
    {
        for (int i = 0; i < 4; i++)
            putc(' ');
        break;
    }
    default:
    {
        if (POS_INDX(VGA_HEIGHT - 1) == VGA_WIDTH)
            new_line();
        write_entry(vga_entry(c, color), POS_INDX(VGA_HEIGHT - 1)++, VGA_HEIGHT - 1);
        break;
    }
    }
}

void Video::new_line()
{
    pos_head = (pos_head +  (sizeof(pos) / sizeof(pos[0])) - 1) % (sizeof(pos) / sizeof(pos[0]));
    POS_INDX(VGA_HEIGHT - 1) = 0;
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
void Video::del_line()
{
    pos_head = pos_head + 1 % (sizeof(pos) / sizeof(pos[0]));
    int y = 0;
    for (y = VGA_HEIGHT - 2; y >= 0; y--)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            this->videomem[(y + 1) * VGA_WIDTH + x] = this->videomem[y * VGA_WIDTH + x];
        }
    }
    VGA_ENTRY v = vga_entry(' ', vga_entry_color(VGA_COLOR::BLACK, VGA_COLOR::BLACK));
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
        this->videomem[x] = v;
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

void Video::printf(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    vprintf(this, fmt, arg);
    va_end(arg);
}
void Video::set_cursor(bool on)
{
    write_entry(on ? vga_entry( '|', WHITE) : vga_entry(' ', BLACK), POS_INDX(VGA_HEIGHT - 1) + 1, VGA_HEIGHT - 1);
}

void Video::debug_pos()
{
    serial::DEBUG_PORT.printf("pos head is %d\n", pos_head);
    for(size_t i = 0; i < VGA_HEIGHT; i++)
    {
        serial::DEBUG_PORT.printf("pos[%d] = %d and actual index is %d\n", i, POS_INDX(i), POS_CALC_INDX(i));
    }
}

Video screen{reinterpret_cast<uint16_t *>(0xffffff800b8000)};
