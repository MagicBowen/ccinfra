#include "gtest/gtest.h"
#include "ccinfra/algo/bits.h"

TEST(BitsTest, shoud_calc_the_correct_bit_mask)
{
    ASSERT_EQ(0,                  BIT_MASK(0));
    ASSERT_EQ(1,                  BIT_MASK(1));
    ASSERT_EQ(0x3,                BIT_MASK(2));
    ASSERT_EQ(0x7F,               BIT_MASK(7));
    ASSERT_EQ(0xFF,               BIT_MASK(8));
    ASSERT_EQ(0x7FFF,             BIT_MASK(15));
    ASSERT_EQ(0xFFFF,             BIT_MASK(16));
    ASSERT_EQ(0x7FFFFFFF,         BIT_MASK(31));
    ASSERT_EQ(0xFFFFFFFF,         BIT_MASK(32));
//    ASSERT_EQ(0x7FFFFFFFFFFFFFFF, BIT_MASK(63));
//    ASSERT_EQ(0xFFFFFFFFFFFFFFFF, BIT_MASK(64));
}
