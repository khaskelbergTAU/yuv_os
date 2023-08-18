#include "page_list.h"


namespace paging
{
void PageList::insert_first(Page* page)
{
    PageListNode* page_node = reinterpret_cast<PageListNode*>(page);
    if (m_head == nullptr) // empty list
    {
        m_head = page_node;
        m_tail = page_node;
        page_node->m_next = nullptr;
        page_node->m_prev = nullptr;
        return;
    }
    m_head->m_prev = page_node;
    page_node->m_next = m_head;
    page_node->m_prev = nullptr;
    m_head = page_node;
}

void PageList::unlink(PageListNode* node)
{
    if (m_head == node)
    {
        m_head = node->m_next;
    }
    if (m_tail == node)
    {
        m_tail = node->m_prev;
    }
    if (node != m_head && node != m_tail)
    {
        PageListNode* prev = node->m_prev;
        PageListNode* next = node->m_next;
        prev->m_next = next;
        next->m_prev = prev;
    }
}

Page *PageList::pop()
{
    YUV_ASSERT(m_head != nullptr, "cant pop an empty list dummy\n");
    Page *page = reinterpret_cast<Page *>(m_head);
    m_head = m_head->m_next;
    m_head->m_prev = nullptr;
    return page;
}

bool PageList::empty()
{
    return m_head == nullptr;
}

} // namespace paging