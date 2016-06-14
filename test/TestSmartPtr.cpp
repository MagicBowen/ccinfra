#include "magellan/magellan.hpp"
#include <ccinfra/mem/SharedObject.h>
#include <ccinfra/mem/SmartPtr.h>
#include <ccinfra/base/Keywords.h>

USING_HAMCREST_NS

namespace
{
    struct Foo : SharedObject
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

FIXTURE(SmartPtrTest)
{
	TEST("should_ref_to_the_correct_addr")
	{
		Foo foo;
		SmartPtr<Foo> pf(&foo);

		ASSERT_THAT(pf.isNull(), be_false());
		ASSERT_THAT(pf.raw(), eq(&foo));
	}

	TEST("should_be_equal_when_point_to_same_addr")
	{
		Foo foo;
		SmartPtr<Foo> pf1(&foo);
		SmartPtr<Foo> pf2(&foo);

		ASSERT_THAT(pf1 == pf2, be_true());
	}

	TEST("should_increase_ref_when_assign_to_smart_ptr")
	{
		Foo foo;

		{
			SmartPtr<Foo> pf(&foo);
			ASSERT_THAT(foo.getRefCount(), eq(1));
			ASSERT_THAT(foo.getDestroyNum(), eq(0));
		}

		ASSERT_THAT(foo.getRefCount(), eq(0));
		ASSERT_THAT(foo.getDestroyNum(), eq(1));
	}

	TEST("should_not_destory_when_ref_not_equal_zero")
	{
		Foo foo;

		SmartPtr<Foo> pf1(&foo);

		{
			SmartPtr<Foo> pf2(&foo);
			ASSERT_THAT(foo.getRefCount(), eq(2));
			ASSERT_THAT(foo.getDestroyNum(), eq(0));
		}

		ASSERT_THAT(foo.getRefCount(), eq(1));
		ASSERT_THAT(foo.getDestroyNum(), eq(0));
	}

	TEST("should_add_ref_when_smart_ptr_construct_other_smart_ptr")
	{
		Foo foo;

		SmartPtr<Foo> pf1(&foo);

		{
			SmartPtr<Foo> pf2(pf1);
			ASSERT_THAT(foo.getRefCount(), eq(2));
			ASSERT_THAT(foo.getDestroyNum(), eq(0));
		}

		ASSERT_THAT(foo.getRefCount(), eq(1));
		ASSERT_THAT(foo.getDestroyNum(), eq(0));
	}

	TEST("should_add_ref_when_smart_ptr_assign_to_other_smart_ptr")
	{
		Foo foo;

		SmartPtr<Foo> pf1(&foo);

		{
			SmartPtr<Foo> pf2;
			pf2 = pf1;
			ASSERT_THAT(foo.getRefCount(), eq(2));
			ASSERT_THAT(foo.getDestroyNum(), eq(0));
		}

		ASSERT_THAT(foo.getRefCount(), eq(1));
		ASSERT_THAT(foo.getDestroyNum(), eq(0));
	}
};
