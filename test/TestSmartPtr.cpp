#include "gtest/gtest.h"
#include "ccinfra/memory/SharedObject.h"
#include "ccinfra/memory/SmartPtr.h"

namespace
{
    DEFINE_ROLE(Foo) EXTENDS(SharedObject)
    {
        Foo() : destroyedNum(0)
        {
        }

        int getDestroyNum() const
        {
            return destroyedNum;
        }

        IMPL_ROLE(SharedObject);

    private:
        OVERRIDE(void destroy())
        {
            destroyedNum++;
        }
        OVERRIDE(bool needDestroy())
        {
            return true;
        }

    private:
        int destroyedNum;
    };
}

TEST(SmartPtrTest, should_ref_to_the_correct_addr)
{
    Foo foo;
    SmartPtr<Foo> pf(&foo);

    ASSERT_FALSE(pf.isNull());
    ASSERT_EQ(&foo, pf.raw());
}

TEST(SmartPtrTest, should_be_equal_when_point_to_same_addr)
{
    Foo foo;
    SmartPtr<Foo> pf1(&foo);
    SmartPtr<Foo> pf2(&foo);

    ASSERT_TRUE(pf1 == pf2);
}

TEST(SmartPtrTest, should_increase_ref_when_assign_to_smart_ptr)
{
    Foo foo;

    {
        SmartPtr<Foo> pf(&foo);
        ASSERT_EQ(1, foo.getRefCount());
        ASSERT_EQ(0, foo.getDestroyNum());
    }

    ASSERT_EQ(0, foo.getRefCount());
    ASSERT_EQ(1, foo.getDestroyNum());
}

TEST(SmartPtrTest, should_not_destory_when_ref_not_equal_zero)
{
    Foo foo;

    SmartPtr<Foo> pf1(&foo);

    {
        SmartPtr<Foo> pf2(&foo);
        ASSERT_EQ(2, foo.getRefCount());
        ASSERT_EQ(0, foo.getDestroyNum());
    }

    ASSERT_EQ(1, foo.getRefCount());
    ASSERT_EQ(0, foo.getDestroyNum());
}

TEST(SmartPtrTest, should_add_ref_when_smart_ptr_construct_other_smart_ptr)
{
    Foo foo;

    SmartPtr<Foo> pf1(&foo);

    {
        SmartPtr<Foo> pf2(pf1);
        ASSERT_EQ(2, foo.getRefCount());
        ASSERT_EQ(0, foo.getDestroyNum());
    }

    ASSERT_EQ(1, foo.getRefCount());
    ASSERT_EQ(0, foo.getDestroyNum());
}

TEST(SmartPtrTest, should_add_ref_when_smart_ptr_assign_to_other_smart_ptr)
{
    Foo foo;

    SmartPtr<Foo> pf1(&foo);

    {
        SmartPtr<Foo> pf2;
        pf2 = pf1;
        ASSERT_EQ(2, foo.getRefCount());
        ASSERT_EQ(0, foo.getDestroyNum());
    }

    ASSERT_EQ(1, foo.getRefCount());
    ASSERT_EQ(0, foo.getDestroyNum());
}
