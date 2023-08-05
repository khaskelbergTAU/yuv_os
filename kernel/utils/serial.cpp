#include "serial.h"
#include <stdarg.h>
namespace serial
{
    SerialPort DEBUG_PORT{0x3f8};
    SerialPort::SerialPort(uint16_t portnum) : portnum(portnum)
    {
        using namespace io;

        outb(irq(), 0x0);
        outb(line_control_reg(), 0x80);
        outb(data_reg_lo(), 0x03);
        outb(data_reg_hi(), 0);
        outb(line_control_reg(), 0x03);
        outb(fifo_reg(), 0xc7);
        outb(modem_reg(), 0x0b);
        outb(modem_reg(), 0x1e);
        outb(data_reg_lo(), 0xae);
        is_valid = (inb(data_reg_lo()) == 0xae);
        outb(modem_reg(), 0x0f);
    }
    constexpr inline uint16_t SerialPort::irq()
    {
        return portnum + 1;
    }
    constexpr inline uint16_t SerialPort::data_reg_lo()
    {
        return portnum;
    }
    constexpr inline uint16_t SerialPort::data_reg_hi()
    {
        return portnum + 1;
    }
    constexpr inline uint16_t SerialPort::line_control_reg()
    {
        return portnum + 3;
    }
    constexpr inline uint16_t SerialPort::fifo_reg()
    {
        return portnum + 2;
    }
    constexpr inline uint16_t SerialPort::modem_reg()
    {
        return portnum + 4;
    }
    constexpr inline uint16_t SerialPort::line_status_reg()
    {
        return portnum + 5;
    }

    void SerialPort::putc(char c)
    {
        volatile uint8_t dat = 0;
        do
        {
            dat = io::inb(line_status_reg()) & 0x20;
        } while (dat == 0);
        io::outb(portnum, c);
    }

    void SerialPort::writestr(const char *s)
    {
        while (*s != 0)
        {
            putc(*(s++));
        }
    }

    void SerialPort::printf(const char *fmt, ...)
    {
        va_list arg;
        va_start(arg, fmt);
        vprintf(this, fmt, arg);
        va_end(arg);
    }
}