#ifndef H05B2224D_B926_4FC0_A936_67B52B8A98DC
#define H05B2224D_B926_4FC0_A936_67B52B8A98DC

#include <stddef.h>
#include <stdint.h>

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef unsigned long  U64;

typedef signed char  S8;
typedef signed short S16;
typedef signed int   S32;
typedef signed long  S64;

template<bool IS_64_BIT> struct PtrValueTraits
{
    typedef U64 PtrValue;
};

template<> struct PtrValueTraits<false>
{
    typedef U32 PtrValue;
};

typedef typename PtrValueTraits<(sizeof(int*) > 4)>::PtrValue PTR_VALUE;

#endif

