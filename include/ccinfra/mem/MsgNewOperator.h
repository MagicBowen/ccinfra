#ifndef CCINFRA_MEMORY_MSG_NEW_H
#define CCINFRA_MEMORY_MSG_NEW_H

#include <new>
#include <string.h>
#include "ccinfra/base/Assertions.h"

CUB_NS_BEGIN

template<typename MSG_ALLOCATOR>
struct MsgNewOperator
{
    void* operator new(size_t size) throw()
    {
        void* p = MSG_ALLOCATOR::alloc(size);
        CUB_ASSERT_VALID_PTR_NIL(p);

#ifdef PERFORMANCE_OPTIMIZE
        ::memset(p, 0, size);
#endif
        return p;
    }

    void operator delete(void* p)
    {
        MSG_ALLOCATOR::free(p);
    }

    virtual ~MsgNewOperator() {}
};

CUB_NS_END

#endif

