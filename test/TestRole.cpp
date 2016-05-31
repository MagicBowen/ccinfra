#include "gtest/gtest.h"
#include "ccinfra/dci/Role.h"
#include "ccinfra/base/BaseTypes.h"

namespace
{
    DEFINE_ROLE(Energy)
    {
        ABSTRACT(void consume());
        ABSTRACT(bool isExhausted() const);
    };

    struct HumanEnergy : Energy
    {
        enum
        {
            MAX_CONSUME_TIMES = 10
        };

        HumanEnergy()
        : isHungry(false), consumeTimes(0)
        {
        }

        void eat()
        {
            isHungry = false;
            consumeTimes = 0;
        }

    private:
        OVERRIDE(void consume())
        {
            consumeTimes++;

            if(consumeTimes >= MAX_CONSUME_TIMES)
            {
                isHungry = true;
            }
        }

        OVERRIDE(bool isExhausted() const)
        {
            return isHungry;
        }

    private:
        bool isHungry;
        U8 consumeTimes;
    };

    struct ChargeEnergy : Energy
    {
        enum
        {
            FULL_PERCENT = 100,
            CONSUME_PERCENT = 1
        };

        ChargeEnergy() : percent(FULL_PERCENT)
        {
        }

        void charge()
        {
            percent = FULL_PERCENT;
        }

    private:
        OVERRIDE(void consume())
        {
            percent -= CONSUME_PERCENT;
        }

        OVERRIDE(bool isExhausted() const)
        {
            return percent == 0;
        }

    private:
        U8 percent;
    };

    DEFINE_ROLE(Worker)
    {
        Worker() : produceNum(0)
        {
        }

        void produce()
        {
            if(ROLE(Energy).isExhausted()) return;

            produceNum++;

            ROLE(Energy).consume();
        }

        U32 getProduceNum() const
        {
            return produceNum;
        }

    private:
        U32 produceNum;

    private:
        USE_ROLE(Energy);
    };

    struct Human : Worker
                 , HumanEnergy
    {
    private:
        IMPL_ROLE(Energy);
    };

    struct Robot : Worker
                 , ChargeEnergy
    {
    private:
        IMPL_ROLE(Energy);
    };
}

TEST(RoleTest, should_cast_to_the_public_role_correctly)
{
    Human human;
    SELF(human, Worker).produce();
    ASSERT_EQ(1, SELF(human, Worker).getProduceNum());
    ASSERT_FALSE(SELF(human, Energy).isExhausted());
}
