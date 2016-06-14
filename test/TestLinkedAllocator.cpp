#include "magellan/magellan.hpp"
#include <ccinfra/mem/LinkedAllocator.h>
#include <ccinfra/ctnr/array/ArraySize.h>

USING_HAMCREST_NS

namespace
{
    int array[] = {0, 1, 2, 3, 4};
}

FIXTURE(LinkedAllocatorTest)
{
    LinkedAllocatorTest()
    : allocator(array, MAX_ALLOC_NUM)
    {
    }

    TEST("should_has_free_slot_num_when_init")
    {
    	ASSERT_THAT(allocator.isBusyListEmpty(), be_true());
    	ASSERT_THAT(allocator.isFreeListEmpty(), be_false());
    }

    TEST("should_alloc_ok_when_has_free_slots")
    {
        for(int i = 0; i < MAX_ALLOC_NUM; i++)
        {
            const int* x = allocator.alloc();
            ASSERT_THAT(__NOT_NULL(x), be_true());
            ASSERT_THAT(*x, eq(i));
        }

        const int* x = allocator.alloc();
        ASSERT_THAT(__IS_NULL(x), be_true());
    }

    TEST("should_alloc_ok_when_free_slots")
    {
        for(int i = 0; i < MAX_ALLOC_NUM - 1; i++)
        {
            allocator.alloc();
        }

        const int* x = allocator.alloc();
        ASSERT_THAT(__NOT_NULL(x), be_true());

        const int *y = allocator.alloc();
        ASSERT_THAT(__IS_NULL(y), be_true());

        ASSERT_THAT(allocator.dealloc(*x), eq(CCINFRA_SUCCESS));

        y = allocator.alloc();
        ASSERT_THAT(__NOT_NULL(y), be_true());
        ASSERT_THAT(*y, eq(2));
    }

    const static int MAX_ALLOC_NUM = 3;
    LinkedAllocator<int, ARR_SIZE(array)> allocator;
};


