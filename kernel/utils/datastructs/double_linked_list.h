#ifndef _KERNEL_UTILS_DATASTRUCTS_DOUBLE_LINKED_LIST_H
#define _KERNEL_UTILS_DATASTRUCTS_DOUBLE_LINKED_LIST_H
namespace std
{
template<class T>
class DoubleLinkedList
{
protected:
    T *m_next = nullptr;
    T *m_prev = nullptr;
public:
    T *next()
    {
        return m_next;
    }

    void set_next(T *next)
    {
        m_next = next;
    }

    T *prev()
    {
        return m_prev;
    }

    void set_prev(T *prev)
    {
        m_prev = prev;
    }

    static T *unlink(T *link)
    {
        T *next = link->next;
        T *prev = link->prev;
        if(next != nullptr)
        {
            if(prev != nullptr)
            {
                prev->next = next;
                next->prev = prev;
                return prev;
            }
            next->prev = nullptr;
            return next;
        }
        if(prev != nullptr)
        {
            prev->next = nullptr;
            return prev;
        }
        return link;
    }

};
}

#endif/* _KERNEL_UTILS_DATASTRUCTS_DOUBLE_LINKED_LIST_H */
