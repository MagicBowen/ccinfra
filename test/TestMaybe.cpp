#include "magellan/magellan.hpp"
#include <ccinfra/mem/Maybe.h>

USING_HAMCREST_NS

FIXTURE(MaybeTest)
{
	TEST("should_be_unpresent_when_init")
	{
	    Maybe<int> x;

	    ASSERT_THAT(x.isPresent(), be_false());
	}

	TEST("should_be_present_when_update")
	{
	    Maybe<int> x;

	    x.update(5);

	    ASSERT_THAT(x.isPresent(), be_true());
	    ASSERT_THAT(*x, eq(5));
	}

	TEST("should_be_present_when_force_effective")
	{
	    Maybe<int> x;

	    x.forceEffective();

	    ASSERT_THAT(x.isPresent(), be_true());
	    ASSERT_THAT(*x, eq(0));
	}

	TEST("should_compare_unequal_when_not_effective")
	{
	    Maybe<int> x;
	    Maybe<int> y(2);

	    ASSERT_THAT(x == y, be_false());
	}

	TEST("should_compare_equal_when_effective")
	{
	    Maybe<int> x;
	    Maybe<int> y(2);

	    x.update(2);

	    ASSERT_THAT(x == y, be_true());
	}
};


