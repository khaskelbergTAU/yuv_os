#ifndef _KERNEL_UTILS_SINGLETON_H
#define _KERNEL_UTILS_SINGLETON_H
#include "panic.h"
template <class T>
class UnsafeSingleton
{
    public:
    static T *getInstance()
    {
        static T s_instance;
        return &s_instance;
    }
    protected:
    UnsafeSingleton(){}
    private:
    UnsafeSingleton(UnsafeSingleton &) = delete;
    UnsafeSingleton(const UnsafeSingleton &) = delete;
    UnsafeSingleton &operator=(const UnsafeSingleton) = delete;
    UnsafeSingleton &operator=(const UnsafeSingleton&) = delete;
    UnsafeSingleton &operator=(UnsafeSingleton &&) = delete;
    UnsafeSingleton(UnsafeSingleton &&) = delete;
};

#endif/* _KERNEL_UTILS_SINGELTON_H */
