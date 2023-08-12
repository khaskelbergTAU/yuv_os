#include "string.h"
namespace std
{
size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len++])
    {
    }
    return len - 1;
}

void memcpy(void* dst, const void* src, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        reinterpret_cast<char*>(dst)[i] = reinterpret_cast<const char*>(src)[i];
    }
}

void memset(void* dst, uint8_t val, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        reinterpret_cast<uint8_t*>(dst)[i] = val;
    }
}
}