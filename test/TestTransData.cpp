#include "magellan/magellan.hpp"
#include <ccinfra/mem/TransData.h>
#include <stdlib.h>

USING_HAMCREST_NS

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
                    throw std::exception();
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

FIXTURE(TestInitTransData)
{
    TransData<Object> data;

    TEST("the_state_should_be_correct_for_an_uninitialized_trans_data")
    {
        assertInit();
    }

    TEST("should_in_NEW_state_after_updating")
    {
        data.update(Object(2));

        assertUpdated();
    }

    TEST("should_in_NEW_state_after_force_updating")
    {
        data.forceUpdate();
        data.update(Object(2));

        assertUpdated();
    }

    TEST("should_in_ACTIVE_state_after_updating_then_confirming")
    {
        data.update(Object(2));
        data.confirm();

        ASSERT_THAT(data.isStable(), be_true());

        ASSERT_THAT(data.isPresent(), be_true());
        ASSERT_THAT(data->getValue(), eq(2));

        ASSERT_THAT(data.isOldPresent(), be_false());
        ASSERT_THAT(data.isChanged(), be_false());
        ASSERT_THAT(data.isChanged(true), be_true());
        ASSERT_THAT(alloc_blocks, eq(1));
    }

    TEST("should_return_to_INIT_after_updating_then_reverting")
    {
        data.update(Object(2));
        data.revert();

        assertInit();
    }

    TEST("touch_should_have_no_effect")
    {
        data.touch();

        assertInit();
    }

    TEST("release_should_have_no_effect")
    {
        data.release();

        assertInit();
    }

    TEST("reset_should_have_no_effect")
    {
        data.reset();

        assertInit();
    }

    TEST("should_not_allow_to_modify")
    {
        ASSERT_THAT(data.modify(), ne(CCINFRA_SUCCESS));

        assertInit();
    }

    void assertInit()
    {
    	ASSERT_THAT(data.isStable(), be_true());

    	ASSERT_THAT(data.isPresent(), be_false());
    	ASSERT_THAT(data.isOldPresent(), be_false());
    	ASSERT_THAT(data.isChanged(), be_false());
    	ASSERT_THAT(data.isChanged(true), be_false());
        ASSERT_THAT(alloc_blocks, eq(0));
    }

    void assertUpdated()
    {
    	ASSERT_THAT(data.isStable(), be_false());

    	ASSERT_THAT(data.isPresent(), be_true());
        ASSERT_THAT(data->getValue(), eq(2));

        ASSERT_THAT(data.isOldPresent(), be_false());
        ASSERT_THAT(data.isChanged(), be_true());
        ASSERT_THAT(data.isChanged(true), be_true());
        ASSERT_THAT(alloc_blocks, eq(1));
    }
};



//////////////////////////////////////////////////////////////////////////
FIXTURE(TestActiveTransData)
{
    TransData<Object> data;

    SETUP()
    {
        data.update(Object(2));
        data.confirm();
    }

    void assertInit()
    {
        ASSERT_THAT(data.isStable(), be_true());

        ASSERT_THAT(data.isPresent(), be_false());
        ASSERT_THAT(data.isOldPresent(), be_false());
        ASSERT_THAT(data.isChanged(), be_false());
        ASSERT_THAT(data.isChanged(true), be_false());
        ASSERT_THAT(alloc_blocks, eq(0));
    }

    void update()
    {
        data.update(Object(3));

        ASSERT_THAT(data.isStable(), be_false());

        ASSERT_THAT(data.isPresent(), be_true());
        ASSERT_THAT(data->getValue(), eq(3));

        ASSERT_THAT(data.isOldPresent(), be_true());
        ASSERT_THAT(data.getOldValue().getValue(), eq(2));

        ASSERT_THAT(data.isChanged(), be_true());
        ASSERT_THAT(data.isChanged(true), be_true());
        ASSERT_THAT(alloc_blocks, eq(2));
    }

	TEST("after_reset_should_return_to_idle")
	{
		data.reset();
		assertInit();
	}

	TEST("should_in_MODIFIED_state_after_updating")
	{
		update();
	}

	TEST("should_in_MODIFIED_state_after_force_updating")
	{
		data.forceUpdate();
		update();
	}

	TEST("should_in_ACTIVE_state_after_updating_and_confirming")
	{
		data.update(Object(3));
		data.confirm();

		ASSERT_THAT(data.isStable(), be_true());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(3));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_not_be_changed_after_updating_a_identical_value")
	{
		data.update(Object(2));

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(2));
	}

	TEST("should_be_able_to_modify")
	{
		ASSERT_THAT(data.modify(), eq(CCINFRA_SUCCESS));

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(2));
	}

	TEST("should_in_ACTIVE_state_after_updating_and_reverting")
	{
		data.update(Object(3));
		data.revert();

		ASSERT_THAT(data.isStable(), be_true());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_in_TOUCHED_state_after_touch")
	{
		data.touch();

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_in_RELEASED_state_after_release")
	{
		data.release();

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_false());

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data.getOldValue().getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_false());
		ASSERT_THAT(alloc_blocks, eq(1));

		data.confirm();

		assertInit();
	}

};

//////////////////////////////////////////////////////////////////////////
FIXTURE(TestTouchTransData)
{
    TransData<Object> data;

    SETUP()
    {
        data.update(Object(2));
        data.confirm();
        data.touch();
    }

    void assertInit()
    {
        ASSERT_THAT(data.isStable(), be_true());

        ASSERT_THAT(data.isPresent(), be_false());
        ASSERT_THAT(data.isOldPresent(), be_false());
        ASSERT_THAT(data.isChanged(), be_false());
        ASSERT_THAT(data.isChanged(true), be_false());
        ASSERT_THAT(alloc_blocks, eq(0));
    }


	TEST("should_be_in_shadow_state")
	{
		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));

		data.confirm();

		ASSERT_THAT(data.isStable(), be_true());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_not_allow_modify")
	{
		ASSERT_THAT(data.modify(), ne(CCINFRA_SUCCESS));

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_be_in_shadow_state_after_updating")
	{
		data.update(Object(2));

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data.getOldValue().getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(2));
	}

	TEST("should_use_new_data_after_updating")
	{
		data.update(Object(3));

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(3));

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data.getOldValue().getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(2));
	}

	TEST("should_in_ACTIVE_state_after_confirm")
	{
		data.confirm();

		ASSERT_THAT(data.isStable(), be_true());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_in_ACTIVE_state_after_reverting")
	{
		data.revert();

		ASSERT_THAT(data.isStable(), be_true());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_in_RELEASED_state_after_releasing")
	{
		data.release();

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_false());

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data.getOldValue().getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_false());
		ASSERT_THAT(alloc_blocks, eq(1));

		data.confirm();
		assertInit();
	}

	TEST("should_in_ACTIVE_state_after_releasing_and_reverting")
	{
		data.release();
		data.revert();

		ASSERT_THAT(data.isStable(), be_true());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_in_IDLE_state_after_reset")
	{
		data.reset();
		assertInit();
	}
};

//////////////////////////////////////////////////////////////////////////
FIXTURE(TestReleaseTransData)
{
    TransData<Object> data;

    SETUP()
    {
        data.update(Object(2));
        data.confirm();
        data.release();
    }

    void assertInit()
    {
    	ASSERT_THAT(data.isStable(), be_true());

    	ASSERT_THAT(data.isPresent(), be_false());
    	ASSERT_THAT(data.isOldPresent(), be_false());
    	ASSERT_THAT(data.isChanged(), be_false());
    	ASSERT_THAT(data.isChanged(true), be_false());
        ASSERT_THAT(alloc_blocks, eq(0));
    }

	TEST("should_in_IDLE_state_after_reseting")
	{
		data.reset();
		assertInit();
	}

	TEST("should_in_IDLE_state_after_confirming")
	{
		data.confirm();
		assertInit();
	}

	TEST("should_in_ACTIVE_state_after_reverting")
	{
		data.revert();

		ASSERT_THAT(data.isStable(), be_true());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_in_RELEASED_state")
	{
		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_false());

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data.getOldValue().getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_false());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_in_SHADOWN_state_after_updating")
	{
		data.update(Object(2));

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data.getOldValue().getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(2));
	}

	TEST("should_use_new_value_state_after_updating")
	{
		data.update(Object(3));

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(3));

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data.getOldValue().getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(2));
	}

	TEST("should_in_ACTIVE_state_after_updating_and_reverting")
	{
		data.update(Object(3));
		data.revert();

		ASSERT_THAT(data.isStable(), be_true());

		ASSERT_THAT(data.isPresent(), be_true());
		ASSERT_THAT(data->getValue(), eq(2));

		ASSERT_THAT(data.isOldPresent(), be_false());

		ASSERT_THAT(data.isChanged(), be_false());
		ASSERT_THAT(data.isChanged(true), be_true());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_not_allow_modify")
	{
		ASSERT_THAT(data.modify(), ne(CCINFRA_SUCCESS));
	}

	TEST("should_have_no_effect_for_touching")
	{
		data.touch();

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_false());

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data.getOldValue().getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_false());
		ASSERT_THAT(alloc_blocks, eq(1));
	}

	TEST("should_have_no_effect_for_releasing")
	{
		data.release();

		ASSERT_THAT(data.isStable(), be_false());

		ASSERT_THAT(data.isPresent(), be_false());

		ASSERT_THAT(data.isOldPresent(), be_true());
		ASSERT_THAT(data.getOldValue().getValue(), eq(2));

		ASSERT_THAT(data.isChanged(), be_true());
		ASSERT_THAT(data.isChanged(true), be_false());
		ASSERT_THAT(alloc_blocks, eq(1));
	}
};
