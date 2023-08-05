#include "pic.h"
namespace pic
{
    using namespace inline_asm;
    volatile int calc_me = 0;
    static void __attribute__ ((noinline)) io_wait()
    {
        int sum = calc_me;
        for (int i = 0; i < 1000; i++)
        {
            sum+= i * ((i % 2 == 0) ? 1 : -1);
        }
        calc_me = sum;
    }
    void remap(uint8_t offset1, uint8_t offset2)
    {
        unsigned char a1, a2;

        a1 = inb(PIC1_DATA); // save masks
        a2 = inb(PIC2_DATA);

        outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // starts the initialization sequence (in cascade mode)
        io_wait();
        outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
        io_wait();
        outb(PIC1_DATA, offset1); // ICW2: Master PIC vector offset
        io_wait();
        outb(PIC2_DATA, offset2); // ICW2: Slave PIC vector offset
        io_wait();
        outb(PIC1_DATA, 4); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
        io_wait();
        outb(PIC2_DATA, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
        io_wait();

        outb(PIC1_DATA, ICW4_8086); // ICW4: have the PICs use 8086 mode (and not 8080 mode)
        io_wait();
        outb(PIC2_DATA, ICW4_8086);
        io_wait();

        outb(PIC1_DATA, a1); // restore saved masks.
        outb(PIC2_DATA, a2);
    }
    void mask(uint8_t irq_line)
    {
        uint16_t port;
        uint8_t value;

        if (irq_line < 8)
        {
            port = PIC1_DATA;
        }
        else
        {
            port = PIC2_DATA;
            irq_line -= 8;
        }
        value = inb(port) | (1 << irq_line);
        outb(port, value);
    }
    void unmask(uint8_t irq_line)
    {
        uint16_t port;
        uint8_t value;

        if (irq_line < 8)
        {
            port = PIC1_DATA;
        }
        else
        {
            port = PIC2_DATA;
            irq_line -= 8;
        }
        value = inb(port) & ~(1 << irq_line);
        outb(port, value);
    }
}