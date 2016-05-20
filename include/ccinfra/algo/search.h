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

#endif

