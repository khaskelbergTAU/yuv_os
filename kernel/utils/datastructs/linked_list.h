#ifndef _KERNEL_UTILS_LINKED_LIST_H
#define _KERNEL_UTILS_LINKED_LIST_H
namespace std
{
template<class T>
class LinkedList
{
protected:
    T *m_next = nullptr;
public:
    T *next()
    {
        return m_next;
    }

    void set_next(T *next)
    {
        m_next = next;
    }
};
}

#endif/* _KERNEL_UTILS_LINKED_LIST_H */
