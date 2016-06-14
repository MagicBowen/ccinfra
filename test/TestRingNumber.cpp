#include "magellan/magellan.hpp"
#include <ccinfra/ctnr/number/RingNumber.h>

USING_HAMCREST_NS

FIXTURE(RingNumberTest)
{
	TEST("should_compare_equal_when_ring_number_init_equal")
	{
		RingNumber<U8, 10> r1(1);
		RingNumber<U8, 10> r2(11);

		ASSERT_THAT(r1 == r2, be_true());
	}

	TEST("should_compare_unequal_when_ring_number_init_unequal")
	{
		RingNumber<U8, 10> r1(2);
		RingNumber<U8, 10> r2(11);

		ASSERT_THAT(r1 == r2, be_false());
	}

	TEST("should_compare_equal_when_ring_number_add_to_equal")
	{
		RingNumber<U8, 10> r1(2);
		RingNumber<U8, 10> r2(11);

		ASSERT_THAT(r1 == (++r2), be_true());
	}

	TEST("should_compare_equal_when_ring_number_move_to_equal")
	{
		RingNumber<U8, 10> r1(4);
		RingNumber<U8, 10> r2(11);

		ASSERT_THAT((r1 << 2) == (r2 >> 11), be_true());
	}
};
