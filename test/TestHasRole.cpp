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
        HumanEnergy()
        : isHungry(false), consumeTimes(0)
        {
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
        enum
        {
            MAX_CONSUME_TIMES = 10
        };

        bool isHungry;
        U8 consumeTimes;
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

    DEFINE_ROLE(Human)
    {
        HAS_ROLE(Worker);
    };

    struct HumanObject : Human
                       , private Worker
                       , private HumanEnergy
    {
    private:
        IMPL_ROLE(Worker);
        IMPL_ROLE(EnergyCarrier);
    };

    struct HumanFactory
    {
        static Human* create()
        {
            return new HumanObject;
        }
    };
}

TEST(HasRoleTest, should_visit_all_the_role_correctly_through_has_role_interface)
{
    Human* human = HumanFactory::create();

    human->ROLE(Worker).produce();

    ASSERT_EQ(1, human->ROLE(Worker).getProduceNum());

    delete human;
}
