#include "printf.h"
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
toStrResult utos(uint64_t a)
{
    toStrResult res{};
    if (a == 0)
    {
        res.data[0] = '0';
        return res;
    }
    int indx{};
    uint64_t b = a;
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
        if (padding == 0)
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