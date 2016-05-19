#include "gtest/gtest.h"
#include "ccinfra/memory/PlainTransData.h"

namespace
{
    struct Foo
    {
       int a, b;
    };
}

struct PlainTransDataTest : testing::Test
{
protected:
    virtual void SetUp()
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

protected:
   PlainTransData<int> intData;
   PlainTransData<Foo> fooData;

   Foo foo;
};

TEST_F(PlainTransDataTest, an_uninitialized_trans_data_should_be_invalid)
{
    ASSERT_FALSE(intData.isPresent());
}

TEST_F(PlainTransDataTest, an_uninitialized_trans_data_should_be_null_if_accessed_by_pointer)
{
    ASSERT_TRUE(0 == intData.operator->());
}

TEST_F(PlainTransDataTest, an_uninitialized_trans_data_should_be_null_if_accessed_by_a_constant_pointer)
{
    ASSERT_TRUE(0 == getConstPointer());
}

TEST_F(PlainTransDataTest, once_set_value_it_should_become_valid)
{
    intData.update(10);
    ASSERT_TRUE(intData.isPresent());
}

TEST_F(PlainTransDataTest, once_set_value_it_should_be_able_to_be_accessed_by_reference)
{
    intData.update(10);
    ASSERT_TRUE(10 == getConstReference());
}

TEST_F(PlainTransDataTest, if_revert_a_updated_value_should_become_invalid)
{
    intData.update(10);
    intData.revert();

    ASSERT_FALSE(intData.isPresent());
}

TEST_F(PlainTransDataTest, before_a_updated_value_is_confirmed_if_it_reverted_should_become_invalid_again)
{
    intData.update(10);
    intData.update(20);
    intData.revert();

    ASSERT_FALSE(intData.isPresent());
}

TEST_F(PlainTransDataTest, once_a_updated_value_is_confirmed_it_should_will_become_valid)
{
    intData.update(10);
    intData.confirm();

    ASSERT_TRUE(intData.isPresent());
}

TEST_F(PlainTransDataTest, a_confirmed_value_can_not_be_reverted)
{
    intData.update(10);
    intData.confirm();
    intData.revert();

    ASSERT_TRUE(intData.isPresent());
    ASSERT_TRUE(10 == (*intData));
}

TEST_F(PlainTransDataTest, a_confirmed_value_can_be_updated_again)
{
    intData.update(10);
    intData.confirm();
    intData.update(20);

    ASSERT_TRUE(intData.isPresent());
    ASSERT_TRUE(20 == (*intData));
}

TEST_F(PlainTransDataTest, if_a_confirmed_value_is_updated_again_then_reverting_should_be_able_to_rollback)
{
    intData.update(10);
    intData.confirm();
    intData.update(20);
    intData.revert();

    ASSERT_TRUE(intData.isPresent());
    ASSERT_TRUE(10 == (*intData));
}

TEST_F(PlainTransDataTest, once_a_confirmed_value_is_reset_it_should_become_invalid)
{
    intData.update(10);
    intData.confirm();
    intData.reset();

    ASSERT_FALSE(intData.isPresent());
}

TEST_F(PlainTransDataTest, an_unstable_data_should_not_be_able_to_be_modified)
{
    intData.update(10);

    ASSERT_NE(SUCCESS, intData.modify());
}

TEST_F(PlainTransDataTest, a_stable_data_should_be_able_to_be_modified)
{
    intData.update(10);
    intData.confirm();

    ASSERT_EQ(SUCCESS, intData.modify());
}

TEST_F(PlainTransDataTest, after_clone_for_modification_it_is_value_keep_unchanged)
{
    enableModifiable();
    ASSERT_EQ(10, *intData);
}

TEST_F(PlainTransDataTest, after_clone_for_modification_should_be_able_modify_actually)
{
    enableModifiable();
    (*intData) = 20;

    ASSERT_EQ(20, *intData);
}

TEST_F(PlainTransDataTest, after_modification_if_revert_should_roll_back)
{
    enableModifiable();
    (*intData) = 20;
    intData.revert();

    ASSERT_EQ(10, *intData);
}

TEST_F(PlainTransDataTest, for_an_uninitialized_trans_data_its_old_value_should_be_invalid)
{
    ASSERT_FALSE(fooData.isOldPresent());
}

TEST_F(PlainTransDataTest, for_an_initialized_trans_data_its_old_value_is_still_be_invalid)
{
    fooData.update(foo);
    ASSERT_FALSE(fooData.isOldPresent());
}

TEST_F(PlainTransDataTest, for_an_initialized_PlainTransData_after_confirmation_its_old_value_is_still_be_invalid)
{
    fooData.update(foo);
    fooData.confirm();

    ASSERT_FALSE(fooData.isOldPresent());
}

TEST_F(PlainTransDataTest, after_confirmation_if_update_again_its_old_value_should_be_valid)
{
    fooData.update(foo);
    fooData.confirm();

    foo.a = 1;
    foo.b = 2;

    fooData.update(foo);

    ASSERT_TRUE(fooData.isOldPresent());
}

TEST_F(PlainTransDataTest, after_confirmation_if_update_again_its_old_value_should_be_the_previous_one)
{
    fooData.update(foo);
    fooData.confirm();

    foo.a = 1;
    foo.b = 2;

    fooData.update(foo);

    ASSERT_EQ(10, fooData.getOldValue().a);
    ASSERT_EQ(20, fooData.getOldValue().b);
}

TEST_F(PlainTransDataTest, after_confirmation_if_update_again_its_new_value_should_be_the_new_one)
{
    fooData.update(foo);
    fooData.confirm();

    foo.a = 1;
    foo.b = 2;

    fooData.update(foo);

    ASSERT_EQ(1, fooData->a);
    ASSERT_EQ(2, fooData->b);
}
