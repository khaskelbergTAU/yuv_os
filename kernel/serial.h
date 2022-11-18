#include <stdint.h>
namespace serial
{
    struct toStrResult
    {
        char data[32];
    };
    toStrResult itos(int a);
    toStrResult itox(unsigned int a, int padding = 0);
    toStrResult itob(unsigned int a, int padding = 0);
    class SerialPort
    {
    public:
        SerialPort(uint16_t portnum);
        void writestr(const char *s);
        void putc(char c);
        template <typename T, typename... Args>
        void printf(const char *fmt, T val, Args... args)
        {
            while (*fmt != 0 && *fmt != '%')
            {
                putc(*(fmt++));
            }
            if (*fmt == 0)
                return;
            fmt++;
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
        constexpr inline uint16_t irq();
        constexpr inline uint16_t data_reg();
        constexpr inline uint16_t line_control_reg();
        constexpr inline uint16_t fifo_reg();
        constexpr inline uint16_t modem_reg();
        constexpr inline uint16_t line_status_reg();
        template <typename T>
        int format(const char *fmt_arg, T val)
        {
            int read_amt{};
            int padding = 0;
            switch (*(fmt_arg++))
            {
            case 'd':
                read_amt++;
                writestr(itos((int)val).data);
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
                writestr(itox((int)val, padding).data);
                break;
            case 'p':
                read_amt++;
                writestr(itox((int)val, 16).data);
                break;
            case 's':
                read_amt++;
                writestr((const char *)val);
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
                writestr(itob((int)val, padding).data);
            }
            return read_amt;
        }
    };
};
extern serial::SerialPort DEBUG_PORT;