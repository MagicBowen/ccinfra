#ifndef HEC29B2BD_30CA_438A_B2EB_8309ED632ECA
#define HEC29B2BD_30CA_438A_B2EB_8309ED632ECA

template< typename KEY>
struct HashFn
{
};

inline size_t hash_string(const char* s)
{
    unsigned long __h = 0;
    for (; *s; ++s)
        __h = 5 * __h + *s;
    return size_t(__h);
}

template<>
struct HashFn<char*>
{
    size_t operator()(const char* s) const
    {
        return hash_string(s);
    }
};

template<>
struct HashFn<const char*>
{
    size_t operator()(const char* s) const
    {
        return hash_string(s);
    }
};

////////////////////////////////////////////////////
#define DEF_HASH_FN_ON_DEFAULT(TYPE)            \
template<> struct HashFn<TYPE>                  \
{                                               \
    size_t operator()(TYPE x) const             \
    {                                           \
        return x;                               \
    }                                           \
};

#define DEF_HASH_FN_ON_MOD(TYPE, RANGE)         \
template<> struct HashFn<TYPE>                  \
{                                               \
    HashFn(const TYPE max = RANGE) : m(max) {}  \
    size_t operator()(TYPE x) const             \
    {                                           \
        return x % m;                           \
    }                                           \
private:                                        \
    const unsigned short m;                     \
};

DEF_HASH_FN_ON_DEFAULT(char)
DEF_HASH_FN_ON_MOD(unsigned char, 0xFF)
DEF_HASH_FN_ON_DEFAULT(signed char)

DEF_HASH_FN_ON_DEFAULT(short)
DEF_HASH_FN_ON_MOD(unsigned short, 0xFFFF)

DEF_HASH_FN_ON_DEFAULT(int)
DEF_HASH_FN_ON_MOD(unsigned int, 0xFFFFFFFF)

DEF_HASH_FN_ON_DEFAULT(long)
DEF_HASH_FN_ON_MOD(unsigned long, 0xFFFFFFFFFFFFFFFF)

#endif
