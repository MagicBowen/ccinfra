#include "gtest/gtest.h"
#include <ccinfra/mem/ObjectAllocator.h>
#include <ccinfra/algo/loop.h>

namespace
{
    struct Foo
    {
        Foo(int a) : x(a)
        {
        }

        int getValue() const
        {
            return x;
        }

        DECL_OPERATOR_NEW();

    private:
        int x;
    };

    const U16 MAX_SLOT_NUM = 2;
}

DEF_OBJ_ALLOCATOR(Foo, MAX_SLOT_NUM);

TEST(ObjectAllocatorTest, should_have_correct_free_slot_when_init)
{
    ASSERT_EQ(MAX_SLOT_NUM, FooAllocator.getFreeSlotNum());
}

TEST(ObjectAllocatorTest, should_alloc_OK_when_has_free_slot)
{
    Foo* foo = new Foo(3);
    ASSERT_TRUE(__NOT_NULL(foo));
    ASSERT_EQ(MAX_SLOT_NUM - 1, FooAllocator.getFreeSlotNum());

    delete foo;
    ASSERT_EQ(MAX_SLOT_NUM, FooAllocator.getFreeSlotNum());
}

TEST(ObjectAllocatorTest, should_not_alloc_when_has_no_free_slot)
{
    Foo* foos[MAX_SLOT_NUM] = {__null_ptr};

    FOR_EACH_0(i, MAX_SLOT_NUM)
    {
        foos[i] = new Foo(0);
    }

    Foo* foo = new Foo(0);
    ASSERT_TRUE(__IS_NULL(foo));

    FOR_EACH_0(i, MAX_SLOT_NUM)
    {
        ASSERT_TRUE(__NOT_NULL(foos[i]));
        delete foos[i];
    }

    foo = new Foo(0);
    ASSERT_TRUE(__NOT_NULL(foo));
    delete foo;
}
