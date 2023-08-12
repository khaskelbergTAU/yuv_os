#ifndef _KERNEL_UTILS_STRING_H
#define _KERNEL_UTILS_STRING_H

#include <stdint.h>
#include <stddef.h>

namespace std
{
size_t strlen(const char* str);
void memcpy(void* dst, const void* src, size_t len);
void memset(void* dst, uint8_t val, size_t len);
}
#endif