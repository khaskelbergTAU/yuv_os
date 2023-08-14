#ifndef _KERNEL_UTILS_PRINTF_H
#define _KERNEL_UTILS_PRINTF_H
#include <stdarg.h>
#include <stdint.h>

struct toStrResult
{
    char data[128];
};
toStrResult itos(int64_t a);
toStrResult utos(uint64_t a);
toStrResult itox(uint64_t a, int padding = 0);
toStrResult itob(uint64_t a, int padding = 0);

template <class T>
void vprintf(T *printer, const char *fmt, va_list arg)
{
    const char *cur_char;
    uint32_t w;
    uint64_t l;
    int64_t i;
    const char *s;

    for (cur_char = fmt; *(cur_char) != '\0'; cur_char++)
    {
        uint32_t padding = 0;
        while (*cur_char != '%' && *cur_char != '\0')
        {
            printer->putc(*cur_char);
            cur_char++;
        }
        if (*cur_char == 0)
        {
            return;
        }
        cur_char++;
        char fmt_char = *cur_char;
        if ((*(cur_char + 1) == '.') && (*(cur_char + 2) == ':'))
        {
            cur_char += 3;
            while (*(cur_char) <= '9' && *(cur_char) >= '0')
            {
                padding *= 10;
                padding += *(cur_char) - '0';
                cur_char++;
            }
            cur_char--;
            if (*cur_char == 0)
            {
                return;
            }
        }
        switch (fmt_char)
        {
        case 'c':
            w = va_arg(arg, int);
            printer->putc(w);
            break;
        case 'd':
            i = va_arg(arg, int64_t);
            printer->writestr(itos(i).data);
            break;
        case 'u':
            l = va_arg(arg, uint64_t);
            printer->writestr(utos(l).data);
            break;
        case 'x':
            l = va_arg(arg, uint64_t);
            printer->writestr(itox(l, padding).data);
            break;
        case 'b':
            l = va_arg(arg, uint64_t);
            printer->writestr(itob(l, padding).data);
            break;
        case 'p':
            l = va_arg(arg, uint64_t);
            printer->writestr(itox(l, sizeof(void *) * 2).data);
            break;
        case 's':
            s = va_arg(arg, const char *);
            printer->writestr(s);
            break;
        case '%':
            printer->putc('%');
            break;
        default:
            printer->writestr("Invalid printf specifier!!!\n");
            break;
        }
    }
}
#endif /* _KERNEL_UTILS_PRINTF_H */
