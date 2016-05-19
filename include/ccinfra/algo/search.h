#ifndef CCINFRA_ALGO_SEARCH_H
#define CCINFRA_ALGO_SEARCH_H

#include <algorithm>

template <typename ForwardIterator, typename T>
ForwardIterator ccinfra_binary_search(ForwardIterator first, ForwardIterator last, const T& val)
{
    ForwardIterator iter = std::lower_bound(first, last, val);
    if (iter != last && *iter == val) return iter;
    else return last;
}

template <typename ForwardIterator, typename T>
ForwardIterator ccinfra_binary_search_nearest(ForwardIterator first, ForwardIterator last, const T& val)
{
    if(val >= *last) return last;
    ForwardIterator iter = std::lower_bound(first, last, val);
    if(iter == first) return first;
    return (*iter - val) > ( val - *(iter-1)) ? (iter-1) : iter;
}

template <typename ForwardIterator, typename T>
ForwardIterator ccinfra_binary_search_lower(ForwardIterator first, ForwardIterator last, const T& val)
{
    if(val >= *last) return last;
    ForwardIterator iter = std::lower_bound(first, last, val);
    if(iter == first) return first;
    return val  - *iter > 0 ? (iter-1) : iter;
}

#endif

