#ifndef CCINFRA_ALGO_BOUND_H
#define CCINFRA_ALGO_BOUND_H

#include "ccinfra/base/BaseTypes.h"

template<typename T>
U32 ccinfra_lower_bound(const  T array[], U32 len, T key)
{
    U32 low  = 0;
    U32 high = len - 1;
    U32 mid  = 0;

    if(key < array[low])
    {
        return low;
    }

    if(key > array[high])
    {
        return high;
    }

    while(low <= high)
    {
        mid = (low + high) >> 1;
        if(key == array[mid])
        {
            return mid;
        }

        if(key > array[mid])
        {
            low  = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return high;
}

template<typename T>
U32 ccinfra_upper_bound(const  T array[], U32 len, T key)
{
    U32 low  = 0;
    U32 high = len - 1;
    U32 mid  = 0;

    if(key < array[low])
    {
        return low;
    }

    if(key > array[high])
    {
        return high;
    }

    while(low <= high)
    {
        mid = (low + high) >> 1;
        if(key == array[mid])
        {
            return mid;
        }

        if(key > array[mid])
        {
            low  = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return low;
}

#endif
