#ifndef _KERNEL_UTILS_ALGORITHM_H
#define _KERNEL_UTILS_ALGORITHM_H
namespace std
{
template<class T>
T& max(T& a, T& b)
{
    return a > b ? a : b;
}

template<class T>
T& min(T& a, T& b)
{
    return a > b ? b : a;
}

}

#endif/* _KERNEL_UTILS_ALGORITHM_H */
