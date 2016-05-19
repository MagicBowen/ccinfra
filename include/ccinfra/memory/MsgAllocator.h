#ifndef CCINFRA_MEMORY_MSG_ALLOCATOR_H
#define CCINFRA_MEMORY_MSG_ALLOCATOR_H

#include <stddef.h>

struct MsgAllocator
{
    static void* alloc(size_t size);
    static void  free(void* p);
    static bool withIn(void* p);
};

#endif

