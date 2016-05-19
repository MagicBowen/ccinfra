#include "gtest/gtest.h"
#include <ccinfra/memory/LinkedAllocator.h>
#include <ccinfra/container/array/ArraySize.h>

namespace
{
    int array[] = {0, 1, 2, 3, 4};
}

struct LinkedAllocatorTest : testing::Test
{
protected:
    const static int MAX_ALLOC_NUM = 3;
    LinkedAllocator<int, ARR_SIZE(array)> allocator{array, MAX_ALLOC_NUM};
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
        ASSERT_TRUE(__notnull__(x));
        ASSERT_EQ(i, *x);
    }

    const int* x = allocator.alloc();
    ASSERT_TRUE(__null__(x));
}

TEST_F(LinkedAllocatorTest, should_alloc_ok_when_free_slots)
{
    for(int i = 0; i < MAX_ALLOC_NUM - 1; i++)
    {
        allocator.alloc();
    }

    const int* x = allocator.alloc();
    ASSERT_TRUE(__notnull__(x));

    const int *y = allocator.alloc();
    ASSERT_TRUE(__null__(y));

    ASSERT_TRUE(CCINFRA_SUCCESS == allocator.dealloc(*x));

    y = allocator.alloc();
    ASSERT_TRUE(__notnull__(y));
    ASSERT_EQ(2, *y);
}
