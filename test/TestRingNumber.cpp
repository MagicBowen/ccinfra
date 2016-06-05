#include "gtest/gtest.h"
#include <ccinfra/ctnr/number/RingNumber.h>

TEST(RingNumberTest, should_compare_equal_when_ring_number_init_equal)
{
    RingNumber<U8, 10> r1(1);
    RingNumber<U8, 10> r2(11);

    ASSERT_TRUE(r1 == r2);
}

TEST(RingNumberTest, should_compare_unequal_when_ring_number_init_unequal)
{
    RingNumber<U8, 10> r1(2);
    RingNumber<U8, 10> r2(11);

    ASSERT_FALSE(r1 == r2);
}

TEST(RingNumberTest, should_compare_equal_when_ring_number_add_to_equal)
{
    RingNumber<U8, 10> r1(2);
    RingNumber<U8, 10> r2(11);

    ASSERT_TRUE(r1 == (++r2));
}

TEST(RingNumberTest, should_compare_equal_when_ring_number_move_to_equal)
{
    RingNumber<U8, 10> r1(4);
    RingNumber<U8, 10> r2(11);

    ASSERT_TRUE((r1 << 2) == (r2 >> 11));
}
