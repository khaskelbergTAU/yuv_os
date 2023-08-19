#ifndef _KERNEL_MEMORY_MEM_H
#define _KERNEL_MEMORY_MEM_H
inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() { };

#endif/* _KERNEL_MEMORY_MEM_H */
