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
        private:
        PageListNode* m_prev = nullptr;
        PageListNode* m_next = nullptr;
        public:
        void set_next(PageListNode *next) {m_next = next;}
        void set_prev(PageListNode *prev) {m_prev = prev;}
        PageListNode *get_prev() {return m_prev;}
        PageListNode *get_next() {return m_next;}
        friend class PageList;
    };
    PageListNode* m_head = nullptr;
    PageListNode* m_tail = nullptr;
    public:
    void insert_first(Page* page);

    void unlink(PageListNode* node);

    bool empty();
    Page* pop();
    PageListNode* head() { return m_head; }
    PageListNode* tail() { return m_tail; }

};

} // namespace paging

#endif/* _KERNEL_MEMORY_PAGE_LIST_H */
