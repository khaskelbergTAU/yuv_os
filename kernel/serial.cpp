#include "serial.h"
namespace serial
{


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

    void SerialPort::printf(const char *fmt)
    {
        writestr(fmt);
    }

    toStrResult itos(int64_t a)
    {
        toStrResult res{};
        if (a == 0)
        {
            res.data[0] = '0';
            return res;
        }
        int indx{};
        if (a < 0)
        {
            res.data[indx++] = '-';
            a = -a;
        }
        int64_t b = a;
        while (b > 0)
        {
            indx++;
            b /= 10;
        }
        while (a > 0)
        {
            res.data[--indx] = (a % 10) + '0';
            a /= 10;
        }
        return res;
    }
    const char *hex_digit_table = "0123456789ABCDEF";
    toStrResult itox(uint64_t x, int padding)
    {
        toStrResult res{};
        int indx = 0;
        res.data[indx++] = '0';
        res.data[indx++] = 'x';
        if (x == 0)
        {
            while (indx < padding + 2)
            {
                res.data[indx++] = '0';
            }

            return res;
        }
        uint64_t b = x;
        while (b > 0)
        {
            indx++;
            padding--;
            b /= 16;
        }
        while (padding > 0)
        {
            res.data[1 + (padding--)] = '0';
            indx++;
        }
        while (x > 0)
        {
            res.data[--indx] = hex_digit_table[x % 16];
            x /= 16;
        }

        return res;
    }
    toStrResult itob(uint64_t x, int padding)
    {
        toStrResult res{};
        int indx{};
        res.data[indx++] = '0';
        res.data[indx++] = 'b';
        if (x == 0)
        {
            while (indx < padding)
            {
                res.data[indx++] = '0';
            }

            return res;
        }
        int b = x;
        while (b > 0)
        {
            indx++;
            padding--;
            b /= 2;
        }
        while (padding > 0)
        {
            res.data[1 + (padding--)] = '0';
            indx++;
        }
        while (x > 0)
        {
            res.data[--indx] = x & 1 ? '1' : '0';
            x /= 2;
        }

        return res;
    }
}