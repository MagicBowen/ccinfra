#ifndef CCINFRA_ALGO_BOUND_H
#define CCINFRA_ALGO_BOUND_H

#include "ccinfra/base/BaseTypes.h"

namespace details
{
    template <typename T, bool isLowBound>
    size_t ccinfra_bound(const T* array, size_t len, T key)
    {
        size_t low  = 0;
        size_t high = len - 1;
        size_t mid  = 0;

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
        return isLowBound? high : low;
    }
}

template<typename T>
size_t ccinfra_lower_bound(const T* array, size_t len, T key)
{
    return details::ccinfra_bound<T, true>(array, len, key);
}

template<typename T>
size_t ccinfra_upper_bound(const T* array, size_t len, T key)
{
    return details::ccinfra_bound<T, false>(array, len, key);
}

#endif
