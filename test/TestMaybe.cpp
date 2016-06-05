#include "gtest/gtest.h"
#include <ccinfra/mem/Maybe.h>

TEST(MaybeTest, should_be_unpresent_when_init)
{
    Maybe<int> x;

    ASSERT_FALSE(x.isPresent());
}

TEST(MaybeTest, should_be_present_when_update)
{
    Maybe<int> x;

    x.update(5);

    ASSERT_TRUE(x.isPresent());
    ASSERT_EQ(5, *x);
}

TEST(MaybeTest, should_be_present_when_force_effective)
{
    Maybe<int> x;

    x.forceEffective();

    ASSERT_TRUE(x.isPresent());
    ASSERT_EQ(0, *x);
}

TEST(MaybeTest, should_compare_unequal_when_not_effective)
{
    Maybe<int> x;
    Maybe<int> y(2);

    ASSERT_FALSE(x == y);
}

TEST(MaybeTest, should_compare_equal_when_effective)
{
    Maybe<int> x;
    Maybe<int> y(2);

    x.update(2);

    ASSERT_TRUE(x == y);
}
