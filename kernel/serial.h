#include <stdint.h>
namespace serial
{
    struct toStrResult
    {
        char data[32];
    };
    toStrResult itos(int a);
    toStrResult itox(unsigned int a);
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
            switch (*(fmt_arg++))
            {
            case 'd':
                read_amt++;
                writestr(itos(val).data);
                break;
            case 'x':
                read_amt++;
                writestr(itox(val).data);
                break;
            }
            return read_amt;
        }
    };
};
extern serial::SerialPort DEBUG_PORT;