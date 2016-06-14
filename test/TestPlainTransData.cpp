#include "magellan/magellan.hpp"
#include <ccinfra/mem/PlainTransData.h>

USING_HAMCREST_NS

namespace
{
    struct Foo
    {
       int a, b;
    };
}

FIXTURE(PlainTransDataTest)
{
	SETUP()
	{
		foo.a = 10;
		foo.b = 20;
	}

	const int* getConstPointer() const
	{
	  return intData.operator->();
	}

	const int& getConstReference() const
	{
	  return *intData;
	}

	void enableModifiable()
	{
	   intData.update(10);
	   intData.confirm();
	   intData.modify();
	}

	PlainTransData<int> intData;
	PlainTransData<Foo> fooData;

	Foo foo;

	TEST("an_uninitialized_trans_data_should_be_invalid")
	{
		ASSERT_THAT(intData.isPresent(), be_false());
	}

	TEST("an_uninitialized_trans_data_should_be_null_if_accessed_by_pointer")
	{
		ASSERT_THAT(0 == intData.operator->(), be_true());
	}

	TEST("an_uninitialized_trans_data_should_be_null_if_accessed_by_a_constant_pointer")
	{
		ASSERT_THAT(0 == getConstPointer(), be_true());
	}

	TEST("once_set_value_it_should_become_valid")
	{
		intData.update(10);
		ASSERT_THAT(intData.isPresent(), be_true());
	}

	TEST("once_set_value_it_should_be_able_to_be_accessed_by_reference")
	{
		intData.update(10);
		ASSERT_THAT(10 == getConstReference(), be_true());
	}

	TEST("if_revert_a_updated_value_should_become_invalid")
	{
		intData.update(10);
		intData.revert();

		ASSERT_THAT(intData.isPresent(), be_false());
	}

	TEST("before_a_updated_value_is_confirmed_if_it_reverted_should_become_invalid_again")
	{
		intData.update(10);
		intData.update(20);
		intData.revert();

		ASSERT_THAT(intData.isPresent(), be_false());
	}

	TEST("once_a_updated_value_is_confirmed_it_should_will_become_valid")
	{
		intData.update(10);
		intData.confirm();

		ASSERT_THAT(intData.isPresent(), be_true());
	}

	TEST("a_confirmed_value_can_not_be_reverted")
	{
		intData.update(10);
		intData.confirm();
		intData.revert();

		ASSERT_THAT(intData.isPresent(), be_true());
		ASSERT_THAT(10 == (*intData), be_true());
	}

	TEST("a_confirmed_value_can_be_updated_again")
	{
		intData.update(10);
		intData.confirm();
		intData.update(20);

		ASSERT_THAT(intData.isPresent(), be_true());
		ASSERT_THAT(20 == (*intData), be_true());
	}

	TEST("if_a_confirmed_value_is_updated_again_then_reverting_should_be_able_to_rollback")
	{
		intData.update(10);
		intData.confirm();
		intData.update(20);
		intData.revert();

		ASSERT_THAT(intData.isPresent(), be_true());
		ASSERT_THAT(10 == (*intData), be_true());
	}

	TEST("once_a_confirmed_value_is_reset_it_should_become_invalid")
	{
		intData.update(10);
		intData.confirm();
		intData.reset();

		ASSERT_THAT(intData.isPresent(), be_false());
	}

	TEST("an_unstable_data_should_not_be_able_to_be_modified")
	{
		intData.update(10);

		ASSERT_THAT(intData.modify(), ne(CCINFRA_SUCCESS));
	}

	TEST("a_stable_data_should_be_able_to_be_modified")
	{
		intData.update(10);
		intData.confirm();

		ASSERT_THAT(intData.modify(), eq(CCINFRA_SUCCESS));
	}

	TEST("after_clone_for_modification_it_is_value_keep_unchanged")
	{
		enableModifiable();
		ASSERT_THAT(*intData, eq(10));
	}

	TEST("after_clone_for_modification_should_be_able_modify_actually")
	{
		enableModifiable();
		(*intData) = 20;

		ASSERT_THAT(*intData, eq(20));
	}

	TEST("after_modification_if_revert_should_roll_back")
	{
		enableModifiable();
		(*intData) = 20;
		intData.revert();

		ASSERT_THAT(*intData, eq(10));
	}

	TEST("for_an_uninitialized_trans_data_its_old_value_should_be_invalid")
	{
		ASSERT_THAT(fooData.isOldPresent(), be_false());
	}

	TEST("for_an_initialized_trans_data_its_old_value_is_still_be_invalid")
	{
		fooData.update(foo);
		ASSERT_THAT(fooData.isOldPresent(), be_false());
	}

	TEST("for_an_initialized_PlainTransData_after_confirmation_its_old_value_is_still_be_invalid")
	{
		fooData.update(foo);
		fooData.confirm();

		ASSERT_THAT(fooData.isOldPresent(), be_false());
	}

	TEST("after_confirmation_if_update_again_its_old_value_should_be_valid")
	{
		fooData.update(foo);
		fooData.confirm();

		foo.a = 1;
		foo.b = 2;

		fooData.update(foo);

		ASSERT_THAT(fooData.isOldPresent(), be_true());
	}

	TEST("after_confirmation_if_update_again_its_old_value_should_be_the_previous_one")
	{
		fooData.update(foo);
		fooData.confirm();

		foo.a = 1;
		foo.b = 2;

		fooData.update(foo);

		ASSERT_THAT(fooData.getOldValue().a, eq(10));
		ASSERT_THAT(fooData.getOldValue().b, eq(20));
	}

	TEST("after_confirmation_if_update_again_its_new_value_should_be_the_new_one")
	{
		fooData.update(foo);
		fooData.confirm();

		foo.a = 1;
		foo.b = 2;

		fooData.update(foo);

		ASSERT_THAT(fooData->a, eq(1));
		ASSERT_THAT(fooData->b, eq(2));
	}
};
