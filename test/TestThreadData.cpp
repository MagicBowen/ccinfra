#include "gtest/gtest.h"
#include <ccinfra/sched/ThreadData.h>

namespace
{
    struct ThreadInfo
    {
        static unsigned int getCurrentId()
        {
            return index;
        }

        static void setCurrentId(unsigned int id)
        {
            index = id;
        }

        const static unsigned int MAX_THREAD_NUM = 2;

    private:
        static unsigned int index;
    };

    unsigned int ThreadInfo::index = 0;
}

struct ThreadDataTest : testing::Test
{
protected:
    ThreadData<int, ThreadInfo> data;
};

TEST_F(ThreadDataTest, should_write_the_correct_thread_data_when_default_thread_id)
{
    *data = 2;

    ASSERT_EQ(2, *data);
    ASSERT_EQ(2, data[0]);
}

TEST_F(ThreadDataTest, should_write_the_correct_thread_data_when_change_the_current_thread_id)
{
    ThreadInfo::setCurrentId(1);

    *data = 5;

    ASSERT_EQ(5, *data);
    ASSERT_EQ(5, data[1]);
}
