#ifndef AVR_MEMORY_ALLOC_HPP
#define AVR_MEMORY_ALLOC_HPP

#include <stdlib.h>
#include <cstdint>

inline void *operator new(std::size_t size) { return malloc(size); }
inline void *operator new(std::size_t, void *ptr) { return ptr; }
inline void operator delete(void *mem) { free(mem); }
inline void operator delete(void *mem, size_t) { free(mem); }

#endif