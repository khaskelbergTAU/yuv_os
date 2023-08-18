#ifndef _KERNEL_UTILS_DATASTRUCTS_DYNAMIC_BITSET_H
#define _KERNEL_UTILS_DATASTRUCTS_DYNAMIC_BITSET_H
#include "../string.h"
#include "panic.h"
namespace std
{
    class dynamic_bitset
    {
        class reference;
        uint64_t *m_set;
        size_t m_bitcount;
        
    public:
        dynamic_bitset() = default;
        dynamic_bitset(size_t bitcount, uint64_t *set) : m_set(set), m_bitcount(bitcount)
        {}
        bool operator[](size_t pos) const;

        reference operator[](size_t pos);
        uint64_t first_set();
        uint64_t first_clear();
        bool is_zero();
    private:
        uint64_t *get_word(size_t pos);
        static uint64_t bitmask(size_t pos);
        class reference
        {
        private:
            uint64_t *m_word_p;
            size_t m_pos;    
        public:
            reference(dynamic_bitset &s, size_t pos);
            reference &operator=(const reference &j);
            reference &operator=(const bool x);
            bool operator~() const;
            operator bool() const;
            reference &flip();
        };
    };


}


#endif/* _KERNEL_UTILS_DATASTRUCTS_DYNAMIC_BITSET_H */
