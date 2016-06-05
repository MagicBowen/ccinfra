#ifndef CCINFRA_CONTAINER_ARRAY_FIX_SIZE_ARRAY_H
#define CCINFRA_CONTAINER_ARRAY_FIX_SIZE_ARRAY_H

#include "ccinfra/base/BaseTypes.h"
#include <string.h>

template <typename T, U32 MAXSIZE, U8 DEFAULT_VALUE = 0>
struct FixSizeArray
{
    FixSizeArray(U32 size = MAXSIZE) : actualSize(size < MAXSIZE? size : MAXSIZE)
    {
    }

    void reset()
    {
         memset(object, DEFAULT_VALUE, sizeof(object));
    }

    const U32 capacity() const
    {
        return actualSize;
    }

    const T& operator[](U32 index) const
    {
        return object[index];
    }

    T& operator[](U32 index)
    {
        return object[index];
    }

private:
    U32 actualSize;
    T object[MAXSIZE];
};

#endif

