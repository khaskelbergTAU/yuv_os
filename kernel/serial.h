#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H
#include <stdint.h>
#include "utils/io_asm.h"
#include "panic.h"
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
        void printf(const char *fmt, ...);

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
    };
};
extern serial::SerialPort DEBUG_PORT;


#endif/* _KERNEL_SERIAL_H */