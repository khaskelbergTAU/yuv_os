#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H
#include <stdint.h>
#include "utils/io_asm.h"
namespace serial
{
    struct toStrResult
    {
        char data[128];
    };
    toStrResult itos(int64_t a);
    toStrResult itox(uint64_t a, int padding = 0);
    toStrResult itob(uint64_t a, int padding = 0);
    class SerialPort
    {
    public:
        SerialPort(uint16_t portnum);
        bool valid(){return is_valid;}
        void writestr(const char *s);
        void putc(char c);
        void printf(const char *fmt);
        template <typename T, typename... Args>
        void printf(const char *fmt, T val, Args... args)
        {
            while (*fmt != 0)
            {
                if(*(fmt) == '%')
                {
                    fmt++;
                    if(*fmt != '%')
                    break;
                }
                putc(*(fmt++));
            }
            if (*fmt == 0)
                return;
            fmt += format(fmt, val);
            printf(fmt, args...);
        }
        template <typename T>
        void printf(const char *fmt, T val)
        {

            while (*fmt != 0 && *fmt != '%')
            {
                putc(*(fmt++));
            }
            if (*fmt == 0)
                return;
            fmt++;
            fmt += format(fmt, val);
            writestr(fmt);
            return;
        }

    private:
        uint16_t portnum;
        bool is_valid;
        constexpr inline uint16_t irq();
        constexpr inline uint16_t data_reg_lo();
        constexpr inline uint16_t data_reg_hi();
        constexpr inline uint16_t line_control_reg();
        constexpr inline uint16_t fifo_reg();
        constexpr inline uint16_t modem_reg();
        constexpr inline uint16_t line_status_reg();
        template <typename T>
        int format(const char *fmt_arg, T val)
        {
            int read_amt = 0;
            int padding = 0;
            uint64_t ext_val = static_cast<uint64_t>((uintptr_t)val); // this truncates 32 bit values. fml
            switch (*(fmt_arg++))
            {
            case 'd':
                read_amt++;
                writestr(itos(*reinterpret_cast<int64_t *>(&ext_val)).data);
                break;
            case 'x':
                read_amt++;
                if (*(fmt_arg) == ':')
                {
                    fmt_arg++;
                    read_amt++;
                    while (*(fmt_arg) <= '9' && *(fmt_arg) >= '0')
                    {
                        padding *= 10;
                        padding += *(fmt_arg) - '0';
                        read_amt++;
                        fmt_arg++;
                    }
                }
                writestr(itox(ext_val, padding).data);
                break;
            case 'p':
                read_amt++;
                writestr(itox(ext_val, sizeof(void *) * 2).data);
                break;
            case 's':
                read_amt++;
                writestr(reinterpret_cast<const char *>(ext_val));
                break;
            case 'b':
                read_amt++;
                if (*(fmt_arg) == ':')
                {
                    fmt_arg++;
                    read_amt++;
                    while (*(fmt_arg) <= '9' && *(fmt_arg) >= '0')
                    {
                        padding *= 10;
                        padding += *(fmt_arg) - '0';
                        read_amt++;
                        fmt_arg++;
                    }
                }
                writestr(itob(ext_val, padding).data);
            }
            return read_amt;
        }
    };
};
extern serial::SerialPort DEBUG_PORT;


#endif/* _KERNEL_SERIAL_H */