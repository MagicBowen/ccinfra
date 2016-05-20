#include "gtest/gtest.h"
#include <ccinfra/algo/bound.h>
#include <ccinfra/container/array/ArraySize.h>

namespace
{
    int a[] = {1, 3, 5, 7};
}

TEST(UpperBoundTest, shoud_find_the_upper_bound_in_middle_position_correct)
{
    ASSERT_EQ(2, ccinfra_upper_bound(a, ARR_SIZE(a), 4));
}

TEST(UpperBoundTest, shoud_find_the_upper_bound_in_left_edge_position_correct)
{
    ASSERT_EQ(0, ccinfra_upper_bound(a, ARR_SIZE(a), 0));
}

TEST(UpperBoundTest, shoud_find_the_upper_bound_in_last_position_correct)
{
    ASSERT_EQ(3, ccinfra_upper_bound(a, ARR_SIZE(a), 7));
}

TEST(UpperBoundTest, shoud_find_the_upper_bound_out_of_right_position_correct)
{
    ASSERT_EQ(3, ccinfra_upper_bound(a, ARR_SIZE(a), 8));
}

TEST(LowerBoundTest, shoud_find_the_upper_bound_in_middle_position_correct)
{
    ASSERT_EQ(1, ccinfra_lower_bound(a, ARR_SIZE(a), 4));
}

TEST(LowerBoundTest, shoud_find_the_upper_bound_in_right_edge_position_correct)
{
    ASSERT_EQ(3, ccinfra_lower_bound(a, ARR_SIZE(a), 8));
}

TEST(LowerBoundTest, shoud_find_the_upper_bound_in_left_edge_position_correct)
{
    ASSERT_EQ(0, ccinfra_lower_bound(a, ARR_SIZE(a), 1));
}

TEST(LowerBoundTest, shoud_find_the_upper_bound_out_of_left_edge_position_correct)
{
    ASSERT_EQ(0, ccinfra_lower_bound(a, ARR_SIZE(a), 0));
}
