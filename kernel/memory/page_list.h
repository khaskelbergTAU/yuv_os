#ifndef _KERNEL_MEMORY_PAGE_LIST_H
#define _KERNEL_MEMORY_PAGE_LIST_H
#include <stdint.h>
#include "panic.h"

#define PAGE_SIZE 0x1000ULL
namespace paging
{
using Page = uint8_t[PAGE_SIZE];
class PageList
{
    struct PageListNode
    {
        PageListNode *m_prev = nullptr;
        PageListNode *m_next = nullptr;
    };
    PageListNode *m_head = nullptr;
    PageListNode *m_tail = nullptr;
public:
    void insert_first(Page *page);

    void unlink(PageListNode *node);

    bool empty();
    Page *pop();

};

} // namespace paging

#endif/* _KERNEL_MEMORY_PAGE_LIST_H */
