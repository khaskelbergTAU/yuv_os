#ifndef _KERNEL_UTILS_STRING_H
#define _KERNEL_UTILS_STRING_H

#include <stdint.h>
#include <stddef.h>
#define ZERO_ARR(arr) do { memset(arr, 0, sizeof(arr) / sizeof(arr[0])); } while(0)
namespace std
{
size_t strlen(const char* str);
void memcpy(void* dst, const void* src, size_t len);
void memset(void* dst, uint8_t val, size_t len);
}
#endif