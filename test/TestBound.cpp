#include "magellan/magellan.hpp"
#include <ccinfra/algo/bound.h>
#include <ccinfra/ctnr/array/ArraySize.h>

USING_HAMCREST_NS

namespace
{
    int a[] = {1, 3, 5, 7};
}

FIXTURE(UpperBoundTest)
{
	TEST("shoud_find_the_upper_bound_in_middle_position_correct")
	{
	    ASSERT_THAT(ccinfra_upper_bound(a, ARR_SIZE(a), 4), eq(2));
	}

	TEST("shoud_find_the_upper_bound_in_left_edge_position_correct")
	{
	    ASSERT_THAT(ccinfra_upper_bound(a, ARR_SIZE(a), 0), eq(0));
	}

	TEST("shoud_find_the_upper_bound_in_last_position_correct")
	{
	    ASSERT_THAT(ccinfra_upper_bound(a, ARR_SIZE(a), 7), eq(3));
	}

	TEST("shoud_find_the_upper_bound_out_of_right_position_correct")
	{
	    ASSERT_THAT(ccinfra_upper_bound(a, ARR_SIZE(a), 8), eq(3));
	}
};

FIXTURE(LowerBoundTest)
{
	TEST("shoud_find_the_upper_bound_in_middle_position_correct")
	{
	    ASSERT_THAT(ccinfra_lower_bound(a, ARR_SIZE(a), 4), eq(1));
	}

	TEST("shoud_find_the_upper_bound_in_right_edge_position_correct")
	{
	    ASSERT_THAT(ccinfra_lower_bound(a, ARR_SIZE(a), 8), eq(3));
	}

	TEST("shoud_find_the_upper_bound_in_left_edge_position_correct")
	{
	    ASSERT_THAT(ccinfra_lower_bound(a, ARR_SIZE(a), 1), eq(0));
	}

	TEST("shoud_find_the_upper_bound_out_of_left_edge_position_correct")
	{
	    ASSERT_THAT(ccinfra_lower_bound(a, ARR_SIZE(a), 0), eq(0));
	}
};


