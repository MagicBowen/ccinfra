#include "gtest/gtest.h"
#include "ccinfra/base/BaseTypes.h"
#include "ccinfra/mem/Placement.h"
#include "ccinfra/base/Keywords.h"
#include "ccinfra/dci/Role.h"
#include <ccinfra/base/NullPtr.h>

namespace
{
    struct Student
    {
        Student(U32 id) : id(id)
        {
        }

        U32 getId() const
        {
            return id;
        }

    private:
        U32 id;
    };
}

TEST(PlacementTest, should_new_object_on_placement_memory)
{
    Placement<Student> studentMemory;

    Student *student = new (studentMemory.alloc()) Student(5);

    ASSERT_EQ(5, student->getId());
    ASSERT_EQ(5, studentMemory.getRef().getId());
    ASSERT_EQ(5, studentMemory->getId());
    ASSERT_EQ(5, (*studentMemory).getId());

    studentMemory.destroy();
}

TEST(PlacementTest, should_new_object_array_on_placement)
{
    const U8 MAX_ENGINE = 5;
    Placement<Student> students[MAX_ENGINE];

    for(int i = 0; i < MAX_ENGINE; i++)
    {
        new (students[i].alloc()) Student(i);
    }

    for(int i = 0; i < MAX_ENGINE; i++)
    {
        ASSERT_EQ(i, students[i]->getId());
    }
}

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
            if(ROLE(Energy).isExhausted()) return;

            produceNum++;

            ROLE(Energy).consume();
        }

        U32 getProduceNum() const
        {
            return produceNum;
        }

        bool isExhausted() const
        {
            return ROLE(Energy).isExhausted();
        }

    private:
        U32 produceNum;

    private:
        USE_ROLE(Energy);
    };

    enum WorkerType
    {
        HUMAN,
        ROBOT
    };

    struct WorkerObject : Worker
    {
        WorkerObject(WorkerType type)
        : energy(__null_ptr__)
        {
            if(type == HUMAN)
            {
                energy = new (energyMemory.human.alloc()) HumanEnergy();
            }
            else
            {
                energy = new (energyMemory.robot.alloc()) ChargeEnergy();
            }
        }

    private:
        union
        {
            Placement<HumanEnergy>  human;
            Placement<ChargeEnergy> robot;
        }energyMemory;

        Energy *energy;

    private:
        IMPL_ROLE_WITH_OBJ(Energy, *energy);
    };
}


TEST(PlacementTest, should_cast_to_the_public_role_correctly_for_human)
{
    WorkerObject human(HUMAN);

    while(!SELF(human, Worker).isExhausted())
    {
        SELF(human, Worker).produce();
    }
    ASSERT_EQ(HumanEnergy::MAX_CONSUME_TIMES, SELF(human, Worker).getProduceNum());
}

TEST(PlacementTest, should_cast_to_the_public_role_correctly_for_robot)
{
    WorkerObject robot(ROBOT);

    while(!SELF(robot, Worker).isExhausted())
    {
        SELF(robot, Worker).produce();
    }
    ASSERT_EQ(ChargeEnergy::FULL_PERCENT / ChargeEnergy::CONSUME_PERCENT,
              SELF(robot, Worker).getProduceNum());
}
