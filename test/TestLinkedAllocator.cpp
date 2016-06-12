#include "gtest/gtest.h"
#include <ccinfra/mem/LinkedAllocator.h>
#include <ccinfra/ctnr/array/ArraySize.h>

namespace
{
    int array[] = {0, 1, 2, 3, 4};
}

struct LinkedAllocatorTest : testing::Test
{
    LinkedAllocatorTest()
    : allocator(array, MAX_ALLOC_NUM)
    {
    }

protected:
    const static int MAX_ALLOC_NUM = 3;
    LinkedAllocator<int, ARR_SIZE(array)> allocator;
};

TEST_F(LinkedAllocatorTest, should_has_free_slot_num_when_init)
{
    ASSERT_TRUE(allocator.isBusyListEmpty());
    ASSERT_FALSE(allocator.isFreeListEmpty());
}

TEST_F(LinkedAllocatorTest, should_alloc_ok_when_has_free_slots)
{
    for(int i = 0; i < MAX_ALLOC_NUM; i++)
    {
        const int* x = allocator.alloc();
        ASSERT_TRUE(__NOT_NULL(x));
        ASSERT_EQ(i, *x);
    }

    const int* x = allocator.alloc();
    ASSERT_TRUE(__IS_NULL(x));
}

TEST_F(LinkedAllocatorTest, should_alloc_ok_when_free_slots)
{
    for(int i = 0; i < MAX_ALLOC_NUM - 1; i++)
    {
        allocator.alloc();
    }

    const int* x = allocator.alloc();
    ASSERT_TRUE(__NOT_NULL(x));

    const int *y = allocator.alloc();
    ASSERT_TRUE(__IS_NULL(y));

    ASSERT_TRUE(CCINFRA_SUCCESS == allocator.dealloc(*x));

    y = allocator.alloc();
    ASSERT_TRUE(__NOT_NULL(y));
    ASSERT_EQ(2, *y);
}
