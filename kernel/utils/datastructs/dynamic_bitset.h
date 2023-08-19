#ifndef _KERNEL_UTILS_DATASTRUCTS_DYNAMIC_BITSET_H
#define _KERNEL_UTILS_DATASTRUCTS_DYNAMIC_BITSET_H
#include "../string.h"
#include "panic.h"
#include "utils/debug.h"
#include "utils/serial.h"
namespace std
{
template<class Allocator>
class dynamic_bitset
{
    class reference;
    uint64_t* m_set;
    size_t m_bitcount;

    public:
    dynamic_bitset() = default;
    dynamic_bitset(size_t bitcount) : m_bitcount(bitcount)
    {
        size_t alloc_size = ROUND_UP((bitcount >> 3), 8);
        m_set = reinterpret_cast<uint64_t *>(Allocator::getInstance()->alloc(alloc_size));
        //serial::DEBUG_PORT.printf("this = %p, &m_set = %p, m_set = %p, bitcount = %x, alloc_size = %x.\n", this, &m_set, m_set, bitcount, alloc_size);

    }
    bool operator[](size_t pos) const
    {
        return m_set[pos / 64] & bitmask(pos);
    }

    reference operator[](size_t pos)
    {
        return reference(*this, pos);
    }
    uint64_t first_set()
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
    uint64_t first_clear()
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
    bool is_zero()
    {
        for (uint64_t indx = 0; indx * 64 < m_bitcount; indx++)
        {
            if (m_set[indx] != 0)
            {
                return false;
            }
        }
        return true;
    }
    void clear()
    {
        for (uint64_t indx = 0; indx * 64 < m_bitcount + 63; indx++)
        {
            m_set[indx] = 0;
            //serial::DEBUG_PORT.printf("this = %p, indx = %d, m_bitcount = %x, &m_set = %p, m_set = %p, writing at %p\n", this, indx, m_bitcount, &m_set, m_set, &m_set[indx]);
        }
    }
    private:
    uint64_t* get_word(size_t pos)
    {
        return &m_set[pos / 64];
    }
    static uint64_t bitmask(size_t pos)
    {
        return 1ULL << pos;
    }
    class reference
    {
        private:
        uint64_t* m_word_p;
        size_t m_pos;
        public:
        reference(dynamic_bitset& s, size_t pos)
        {
            m_word_p = s.get_word(pos);
            m_pos = pos % 64;
        }
        reference& operator=(const dynamic_bitset::reference& j)
        {
            if (*(j.m_word_p) & bitmask(j.m_pos))
                *m_word_p |= bitmask(m_pos);
            else
                *m_word_p &= ~bitmask(m_pos);
            return *this;
        }
        reference& operator=(const bool x)
        {
            if (x)
                *m_word_p |= bitmask(m_pos);
            else
                *m_word_p &= ~bitmask(m_pos);
            return *this;
        }
        bool operator~() const
        {
            return (*m_word_p & bitmask(m_pos)) == 0;
        }
        operator bool() const
        {
            return (*m_word_p & bitmask(m_pos)) != 0;
        }

        reference& flip()
        {
            *m_word_p ^= bitmask(m_pos);
            return *this;
        }
    };
};


}

#endif/* _KERNEL_UTILS_DATASTRUCTS_DYNAMIC_BITSET_H */
