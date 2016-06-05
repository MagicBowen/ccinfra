#include "gtest/gtest.h"
#include <ccinfra/algo/search.h>
#include <ccinfra/ctnr/array/ArraySize.h>

namespace
{
    int a[] = {1, 3, 5, 7};
}

TEST(BinarySearchTest, should_find_correct_when_key_is_in_the_middle_position)
{
    int *r = ccinfra_binary_search(begin_a(a), end_a(a), 5);
    ASSERT_EQ(5, *r);
    ASSERT_EQ(2, position(a, r));
}

TEST(BinarySearchTest, should_return_the_end_when_key_not_find)
{
    ASSERT_EQ(end_a(a), ccinfra_binary_search(begin_a(a), end_a(a), 6));
}
