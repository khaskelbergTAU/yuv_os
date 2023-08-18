
#include "dynamic_bitset.h"
namespace std
{
bool dynamic_bitset::operator[](size_t pos) const
{
    return m_set[pos / 64] & bitmask(pos);
}

dynamic_bitset::reference dynamic_bitset::operator[](size_t pos)
{
    return reference(*this, pos);
}
uint64_t dynamic_bitset::first_set()
{
    uint64_t indx = 0;
    for (indx = 0; indx * 64 < m_bitcount; indx++)
    {
        if (m_set[indx] != 0)
            break;
    }
    YUV_ASSERT(!((indx * 64) >= m_bitcount), "Your set was zero dumbass\n");
    return indx * 64 + __builtin_ctzll(m_set[indx]);
}
uint64_t dynamic_bitset::first_clear()
{
    uint64_t indx = 0;
    for (indx = 0; indx * 64 < m_bitcount; indx++)
    {
        if (m_set[indx] != -1ULL)
            break;
    }
    YUV_ASSERT(!((indx * 64) >= m_bitcount), "Your set doesnt have clear bits dumbass\n");
    return indx * 64 + __builtin_ctzll(~(m_set[indx]));
}
bool dynamic_bitset::is_zero()
{
    for (uint64_t indx = 0; indx * 64 < m_bitcount; indx++)
    {
        if (m_set[indx] != 0)
        {
            screen.printf("not zero at indx %x\n", indx);
            return false;
        }
    }
    return true;
}
uint64_t* dynamic_bitset::get_word(size_t pos)
{
    return &m_set[pos / 64];
}
uint64_t dynamic_bitset::bitmask(size_t pos)
{
    return 1ULL << pos;
}
dynamic_bitset::reference::reference(dynamic_bitset& s, size_t pos)
{
    m_word_p = s.get_word(pos);
    m_pos = pos % 64;
}
dynamic_bitset::reference& dynamic_bitset::reference::operator=(const dynamic_bitset::reference& j)
{
    if (*(j.m_word_p) & bitmask(j.m_pos))
        *m_word_p |= bitmask(m_pos);
    else
        *m_word_p &= ~bitmask(m_pos);
    return *this;
}
dynamic_bitset::reference& dynamic_bitset::reference::operator=(const bool x)
{
    if (x)
        *m_word_p |= bitmask(m_pos);
    else
        *m_word_p &= ~bitmask(m_pos);
    return *this;
}
bool dynamic_bitset::reference::operator~() const
{
    return (*m_word_p & bitmask(m_pos)) == 0;
}
dynamic_bitset::reference::operator bool() const
{
    return (*m_word_p & bitmask(m_pos)) != 0;
}

dynamic_bitset::reference& dynamic_bitset::reference::flip()
{
    *m_word_p ^= bitmask(m_pos);
    return *this;
}
};
