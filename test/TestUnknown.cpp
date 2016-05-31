#include "gtest/gtest.h"
#include "ccinfra/base/BaseTypes.h"
#include "ccinfra/dci/Unknown.h"
#include "ccinfra/base/NullPtr.h"

namespace
{
    UNKNOWN_INTERFACE(Worker, 0x1234)
    {
        Worker() : produceNum(0)
        {
        }

        void produce()
        {
            produceNum++;
        }

        U32 getProduceNum() const
        {
            return produceNum;
        }

    private:
        U32 produceNum;
    };

    UNKNOWN_INTERFACE(SleepInfo, 0x2345)
    {
        SleepInfo() : sleeping(false)
        {
        }

        void sleep()
        {
            sleeping = true;
        }

        void wakeup()
        {
            sleeping = false;
        }

        bool isSleeping() const
        {
            return sleeping;
        }

    private:
        bool sleeping;
    };

    UNKNOWN_INTERFACE(ChargeInfo, 0x3521)
    {
        ChargeInfo() : percent(0)
        {
        }

        void charge()
        {
            percent = 100;
        }

        void cosume()
        {
            percent -= 10;
        }

        bool isNeedCharge() const
        {
            return percent == 0;
        }

    private:
        U8 percent;
    };

    struct Human : dci::Unknown
                 , private Worker
                 , private SleepInfo
    {
        BEGIN_INTERFACE_TABLE()
            __HAS_INTERFACE(Worker)
            __HAS_INTERFACE(SleepInfo)
        END_INTERFACE_TABLE()
    };

    struct Robot : dci::Unknown
                 , private Worker
                 , private ChargeInfo
    {
        BEGIN_INTERFACE_TABLE()
            __HAS_INTERFACE(Worker)
            __HAS_INTERFACE(ChargeInfo)
        END_INTERFACE_TABLE()
    };
}

TEST(UnknownTest, should_cast_to_the_exist_interface)
{
    Human human;
    dci::Unknown * unknown = &human;

    Worker* worker = dci::unknown_cast<Worker>(unknown);
    ASSERT_TRUE(__notnull__(worker));

    worker->produce();
    ASSERT_EQ(1, worker->getProduceNum());

    SleepInfo * info = dci::unknown_cast<SleepInfo>(unknown);
    ASSERT_TRUE(__notnull__(info));

    info->sleep();
    ASSERT_TRUE(info->isSleeping());

    info->wakeup();
    ASSERT_FALSE(info->isSleeping());
}

TEST(UnknownTest, should_return_null_when_cast_to_unexist_interface)
{
    Robot robot;
    dci::Unknown * unknown = &robot;

    Worker* worker = dci::unknown_cast<Worker>(unknown);
    ASSERT_TRUE(__notnull__(worker));

    SleepInfo * info = dci::unknown_cast<SleepInfo>(unknown);
    ASSERT_TRUE(__null__(info));
}
