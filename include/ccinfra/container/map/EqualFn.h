#ifndef H4358042D_C5CA_48CF_82A6_05A9B0B3B8EA
#define H4358042D_C5CA_48CF_82A6_05A9B0B3B8EA

template<typename KEY>
struct EqualFn
{
    bool operator()(const KEY& x, const KEY& y) const
    {
        return x == y;
    }
};

#endif
