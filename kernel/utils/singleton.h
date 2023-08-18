#ifndef _KERNEL_UTILS_SINGLETON_H
#define _KERNEL_UTILS_SINGLETON_H
#include "panic.h"
template <class T>
class UnsafeSingleton
{
    public:
    static T &getInstance()
    {
        static T instance;
        return instance;
    }
};

#endif/* _KERNEL_UTILS_SINGELTON_H */
