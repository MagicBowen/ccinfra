#include "magellan/magellan.hpp"
#include <ccinfra/algo/search.h>
#include <ccinfra/ctnr/array/ArraySize.h>

USING_HAMCREST_NS

namespace
{
    int a[] = {1, 3, 5, 7};
}

FIXTURE(BinarySearchTest)
{
	TEST("should_find_correct_when_key_is_in_the_middle_position")
	{
		int *r = ccinfra_binary_search(begin_a(a), end_a(a), 5);
		ASSERT_THAT(*r, eq(5));
		ASSERT_THAT(position(a, r), eq(2));
	}

	TEST("should_return_the_end_when_key_not_find")
	{
		ASSERT_THAT(ccinfra_binary_search(begin_a(a), end_a(a), 6), eq(end_a(a)));
	}
};
