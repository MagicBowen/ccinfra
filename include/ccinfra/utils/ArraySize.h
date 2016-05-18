#ifndef H29B1191B_457B_4E80_8056_4573626F0A98
#define H29B1191B_457B_4E80_8056_4573626F0A98

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

template <class T, size_t N>
inline T* begin_a(T(&arr)[N])
{
    return arr;
}

template <class T, size_t N>
inline T* end_a(T(&arr)[N])
{
    return arr + N;
}

#endif
