#include "gtest/gtest.h"
#include "ccinfra/dci/Role.h"
#include "ccinfra/base/BaseTypes.h"

namespace
{
    DEFINE_ROLE(EnergyCarrier)
    {
        ABSTRACT(void consume());
        ABSTRACT(bool isExhausted() const);
    };

    struct HumanEnergy : EnergyCarrier
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

    struct ChargeEnergy : EnergyCarrier
    {
        enum
        {
            FULL_PERCENT = 100,
            CONSUME_PERCENT = 1
        };

        ChargeEnergy() : percent(0)
        {
        }

        void charge()
        {
            percent = FULL_PERCENT;
        }

    private:
        OVERRIDE(void consume())
        {
            if(percent > 0)
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
            if(ROLE(EnergyCarrier).isExhausted()) return;

            produceNum++;

            ROLE(EnergyCarrier).consume();
        }

        U32 getProduceNum() const
        {
            return produceNum;
        }

    private:
        U32 produceNum;

    private:
        USE_ROLE(EnergyCarrier);
    };

    struct Human : Worker
                 , HumanEnergy
    {
    private:
        IMPL_ROLE(EnergyCarrier);
    };

    struct Robot : Worker
                 , ChargeEnergy
    {
    private:
        IMPL_ROLE(EnergyCarrier);
    };
}

TEST(RoleTest, should_cast_to_the_public_role_correctly_for_human)
{
    Human human;

    while(!SELF(human, EnergyCarrier).isExhausted())
    {
        SELF(human, Worker).produce();
    }
    ASSERT_EQ(Human::MAX_CONSUME_TIMES, SELF(human, Worker).getProduceNum());

    human.eat();
    ASSERT_FALSE(SELF(human, EnergyCarrier).isExhausted());
}

TEST(RoleTest, should_cast_to_the_public_role_correctly_for_robot)
{
    Robot robot;

    SELF(robot, ChargeEnergy).charge();

    while(!SELF(robot, EnergyCarrier).isExhausted())
    {
        SELF(robot, Worker).produce();
    }
    ASSERT_EQ(ChargeEnergy::FULL_PERCENT / ChargeEnergy::CONSUME_PERCENT,
              SELF(robot, Worker).getProduceNum());
}
