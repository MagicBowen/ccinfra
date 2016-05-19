#include "gtest/gtest.h"
#include "ccinfra/memory/TransData.h"
#include <malloc.h>

namespace
{
    static unsigned int alloc_blocks = 0;

    struct ObjectInfo
    {
        ObjectInfo() : value(0xFF) {}
        ObjectInfo(int value) : value(value) {}

        int getValue() const
        {
            return value;
        }

        bool operator==(const ObjectInfo& rhs) const
        {
            return value == rhs.value;
        }

        bool operator!=(const ObjectInfo& rhs) const
        {
            return value != rhs.value;
        }

        void* operator new(size_t size)
        {
            alloc_blocks++;
            return ::malloc(size);
        }

        void operator delete(void* p)
        {
            if(p != 0)
            {
                if(alloc_blocks == 0) 
                {
                    GTEST_FAIL();
                }

                alloc_blocks--;
                ::free(p);
            }
        }
    private:
        int value;
    };

    struct Object
    {
        Object() : info(0) {}
        Object(int value) : info(new ObjectInfo(value)) {}

        Object& operator=(const Object& rhs)
        {
            info = rhs.info;
            return *this;
        }

        Status copyTo(Object& rhs)
        {
            CCINFRA_ASSERT_VALID_PTR(info);

            if (rhs.info == 0) rhs.info = new ObjectInfo(*info);
            else *rhs.info = *info;

            return CCINFRA_SUCCESS;
        }

        void reset()
        {
            delete info;
            info = 0;
        }

        bool operator==(const Object& rhs) const
        {
            if(info == 0 && rhs.info == 0) return true;

            if(info != 0 && rhs.info != 0)
            {
                return *info == *rhs.info;
            }

            return false;
        }

        bool operator!=(const Object& rhs) const
        {
            return !(*this == rhs);
        }

        int getValue() const
        {
            return info == 0 ? 0xFF : info->getValue();
        }

    private:
        ObjectInfo *info;
    };

}

struct TestInitTransData : testing::Test
{
    TransData<Object> data;

    void assertInit()
    {
        ASSERT_TRUE(data.isStable());

        ASSERT_FALSE(data.isPresent());
        ASSERT_FALSE(data.isOldPresent());
        ASSERT_FALSE(data.isChanged());
        ASSERT_FALSE(data.isChanged(true));
        ASSERT_EQ(0, alloc_blocks);
    }

    void assertUpdated()
    {
        ASSERT_FALSE(data.isStable());

        ASSERT_TRUE(data.isPresent());
        ASSERT_EQ(2, data->getValue());

        ASSERT_FALSE(data.isOldPresent());
        ASSERT_TRUE(data.isChanged());
        ASSERT_TRUE(data.isChanged(true));
        ASSERT_EQ(1, alloc_blocks);
    }
};

TEST_F(TestInitTransData, the_state_should_be_correct_for_an_uninitialized_trans_data)
{
    assertInit();
}

TEST_F(TestInitTransData, should_in_NEW_state_after_updating)
{
    data.update(Object(2));

    assertUpdated();
}

TEST_F(TestInitTransData, should_in_NEW_state_after_force_updating)
{
    data.forceUpdate();
    data.update(Object(2));

    assertUpdated();
}

TEST_F(TestInitTransData, should_in_ACTIVE_state_after_updating_then_confirming)
{
    data.update(Object(2));
    data.confirm();

    ASSERT_TRUE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());
    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestInitTransData, should_return_to_INIT_after_updating_then_reverting)
{
    data.update(Object(2));
    data.revert();

    assertInit();
}

TEST_F(TestInitTransData, touch_should_have_no_effect)
{
    data.touch();

    assertInit();
}

TEST_F(TestInitTransData, release_should_have_no_effect)
{
    data.release();

    assertInit();
}

TEST_F(TestInitTransData, reset_should_have_no_effect)
{
    data.reset();

    assertInit();
}

TEST_F(TestInitTransData, should_not_allow_to_modify)
{
    ASSERT_NE(CCINFRA_SUCCESS, data.modify());

    assertInit();
}

//////////////////////////////////////////////////////////////////////////
struct TestActiveTransData : testing::Test
{
    TransData<Object> data;

    void SetUp()
    {
        data.update(Object(2));
        data.confirm();
    }

    void assertInit()
    {
        ASSERT_TRUE(data.isStable());

        ASSERT_FALSE(data.isPresent());
        ASSERT_FALSE(data.isOldPresent());
        ASSERT_FALSE(data.isChanged());
        ASSERT_FALSE(data.isChanged(true));
        ASSERT_EQ(0, alloc_blocks);
    }

    void update()
    {
        data.update(Object(3));

        ASSERT_FALSE(data.isStable());

        ASSERT_TRUE(data.isPresent());
        ASSERT_EQ(3, data->getValue());

        ASSERT_TRUE(data.isOldPresent());
        ASSERT_EQ(2, data.getOldValue().getValue());

        ASSERT_TRUE(data.isChanged());
        ASSERT_TRUE(data.isChanged(true));
        ASSERT_EQ(2, alloc_blocks);
    }
};

TEST_F(TestActiveTransData, after_reset_should_return_to_idle)
{
    data.reset();
    assertInit();
}

TEST_F(TestActiveTransData, should_in_MODIFIED_state_after_updating)
{
    update();
}

TEST_F(TestActiveTransData, should_in_MODIFIED_state_after_force_updating)
{
    data.forceUpdate();
    update();
}

TEST_F(TestActiveTransData, should_in_ACTIVE_state_after_updating_and_confirming)
{
    data.update(Object(3));
    data.confirm();

    ASSERT_TRUE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(3, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestActiveTransData, should_not_be_changed_after_updating_a_identical_value)
{
    data.update(Object(2));

    ASSERT_FALSE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(2, alloc_blocks);
}

TEST_F(TestActiveTransData, should_be_able_to_modify)
{
    ASSERT_EQ(CCINFRA_SUCCESS, data.modify());

    ASSERT_FALSE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(2, alloc_blocks);
}

TEST_F(TestActiveTransData, should_in_ACTIVE_state_after_updating_and_reverting)
{
    data.update(Object(3));
    data.revert();

    ASSERT_TRUE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestActiveTransData, should_in_TOUCHED_state_after_touch)
{
    data.touch();

    ASSERT_FALSE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_TRUE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestActiveTransData, should_in_RELEASED_state_after_release)
{
    data.release();

    ASSERT_FALSE(data.isStable());

    ASSERT_FALSE(data.isPresent());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data.getOldValue().getValue());

    ASSERT_TRUE(data.isChanged());
    ASSERT_FALSE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);

    data.confirm();

    assertInit();
}

//////////////////////////////////////////////////////////////////////////
struct TestTouchTransData : testing::Test
{
    TransData<Object> data;

    void SetUp()
    {
        data.update(Object(2));
        data.confirm();
        data.touch();
    }

    void assertInit()
    {
        ASSERT_TRUE(data.isStable());

        ASSERT_FALSE(data.isPresent());
        ASSERT_FALSE(data.isOldPresent());
        ASSERT_FALSE(data.isChanged());
        ASSERT_FALSE(data.isChanged(true));
        ASSERT_EQ(0, alloc_blocks);
    }
};

TEST_F(TestTouchTransData, should_be_in_shadow_state)
{
    ASSERT_FALSE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_TRUE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);

    data.confirm();

    ASSERT_TRUE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestTouchTransData, should_not_allow_modify)
{
    ASSERT_NE(CCINFRA_SUCCESS, data.modify());

    ASSERT_FALSE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_TRUE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestTouchTransData, should_be_in_shadow_state_after_updating)
{
    data.update(Object(2));

    ASSERT_FALSE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data.getOldValue().getValue());

    ASSERT_TRUE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(2, alloc_blocks);
}

TEST_F(TestTouchTransData, should_use_new_data_after_updating)
{
    data.update(Object(3));

    ASSERT_FALSE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(3, data->getValue());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data.getOldValue().getValue());

    ASSERT_TRUE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(2, alloc_blocks);
}

TEST_F(TestTouchTransData, should_in_ACTIVE_state_after_confirm)
{
    data.confirm();

    ASSERT_TRUE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestTouchTransData, should_in_ACTIVE_state_after_reverting)
{
    data.revert();

    ASSERT_TRUE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestTouchTransData, should_in_RELEASED_state_after_releasing)
{
    data.release();

    ASSERT_FALSE(data.isStable());

    ASSERT_FALSE(data.isPresent());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data.getOldValue().getValue());

    ASSERT_TRUE(data.isChanged());
    ASSERT_FALSE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);

    data.confirm();
    assertInit();
}

TEST_F(TestTouchTransData, should_in_ACTIVE_state_after_releasing_and_reverting)
{
    data.release();
    data.revert();

    ASSERT_TRUE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestTouchTransData, should_in_IDLE_state_after_reset)
{
    data.reset();
    assertInit();
}

//////////////////////////////////////////////////////////////////////////
struct TestReleaseTransData : testing::Test
{
    TransData<Object> data;

    void SetUp()
    {
        data.update(Object(2));
        data.confirm();
        data.release();
    }

    void assertInit()
    {
        ASSERT_TRUE(data.isStable());

        ASSERT_FALSE(data.isPresent());
        ASSERT_FALSE(data.isOldPresent());
        ASSERT_FALSE(data.isChanged());
        ASSERT_FALSE(data.isChanged(true));
        ASSERT_EQ(0, alloc_blocks);
    }
};

TEST_F(TestReleaseTransData, should_in_IDLE_state_after_reseting)
{
    data.reset();
    assertInit();
}

TEST_F(TestReleaseTransData, should_in_IDLE_state_after_confirming)
{
    data.confirm();
    assertInit();
}

TEST_F(TestReleaseTransData, should_in_ACTIVE_state_after_reverting)
{
    data.revert();

    ASSERT_TRUE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestReleaseTransData, should_in_RELEASED_state)
{
    ASSERT_FALSE(data.isStable());

    ASSERT_FALSE(data.isPresent());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data.getOldValue().getValue());

    ASSERT_TRUE(data.isChanged());
    ASSERT_FALSE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestReleaseTransData, should_in_SHADOWN_state_after_updating)
{
    data.update(Object(2));

    ASSERT_FALSE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data.getOldValue().getValue());

    ASSERT_TRUE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(2, alloc_blocks);
}

TEST_F(TestReleaseTransData, should_use_new_value_state_after_updating)
{
    data.update(Object(3));

    ASSERT_FALSE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(3, data->getValue());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data.getOldValue().getValue());

    ASSERT_TRUE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(2, alloc_blocks);
}

TEST_F(TestReleaseTransData, should_in_ACTIVE_state_after_updating_and_reverting)
{
    data.update(Object(3));
    data.revert();

    ASSERT_TRUE(data.isStable());

    ASSERT_TRUE(data.isPresent());
    ASSERT_EQ(2, data->getValue());

    ASSERT_FALSE(data.isOldPresent());

    ASSERT_FALSE(data.isChanged());
    ASSERT_TRUE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestReleaseTransData, should_not_allow_modify)
{
    ASSERT_NE(CCINFRA_SUCCESS, data.modify());
}

TEST_F(TestReleaseTransData, should_have_no_effect_for_touching)
{
    data.touch();

    ASSERT_FALSE(data.isStable());

    ASSERT_FALSE(data.isPresent());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data.getOldValue().getValue());

    ASSERT_TRUE(data.isChanged());
    ASSERT_FALSE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}

TEST_F(TestReleaseTransData, should_have_no_effect_for_releasing)
{
    data.release();

    ASSERT_FALSE(data.isStable());

    ASSERT_FALSE(data.isPresent());

    ASSERT_TRUE(data.isOldPresent());
    ASSERT_EQ(2, data.getOldValue().getValue());

    ASSERT_TRUE(data.isChanged());
    ASSERT_FALSE(data.isChanged(true));
    ASSERT_EQ(1, alloc_blocks);
}
