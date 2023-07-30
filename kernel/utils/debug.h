#ifndef _KERNEL_UTILS_DEBUG_H
#define _KERNEL_UTILS_DEBUG_H
#define INFO(a) DEBUG_PORT.printf("[INFO] %s at %s:%d:%s\n", a, __FILE__, __LINE__, __func__);
#endif