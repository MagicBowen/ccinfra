#ifndef H1BEBC79D_5BA8_4F6C_AD8C_94F820EC7D5B
#define H1BEBC79D_5BA8_4F6C_AD8C_94F820EC7D5B

#define BIT_MASK(BIT_NUM)       ((1 << BIT_NUM) - 1)

#define GET_BITS_VALUE(target, offset, length) (((target) >> (offset)) & BIT_MASK(length))

#define IS_BIT_SET(target, offset) (GET_BITS_VALUE(target, offset, 1) > 0)

#endif
