#include "gtest/gtest.h"
#include "ccinfra/memory/ObjectAllocator.h"

namespace
{
    struct Obj
    {
        Obj(int a) : x(a)
        {
        }

        int getValue() const
        {
            return x;
        }

        DECL_OPERATOR_NEW();

    private:
        int x;
    };

    const U16 MAX_SLOT_NUM = 2;
}

DEF_OBJ_ALLOCATOR(Obj, MAX_SLOT_NUM);

TEST(ObjectAllocatorTest, should_have_correct_free_slot_when_init)
{
    ASSERT_EQ(MAX_SLOT_NUM, ObjAllocator.getFreeSlotNum());
}

TEST(ObjectAllocatorTest, should_alloc_OK_when_has_free_slot)
{
    Obj* o = new Obj(3);
    ASSERT_TRUE(__notnull__(o));
    ASSERT_EQ(MAX_SLOT_NUM - 1, ObjAllocator.getFreeSlotNum());

    delete o;
    ASSERT_EQ(MAX_SLOT_NUM, ObjAllocator.getFreeSlotNum());
}

TEST(ObjectAllocatorTest, should_not_alloc_when_has_no_free_slot)
{
    Obj* objects[MAX_SLOT_NUM] = {__null_ptr__};

    for(auto& object : objects)
    {
        object = new Obj(0);
    }

    Obj* object = new Obj(0);
    ASSERT_TRUE(__null__(object));

    for(int i = 0; i < MAX_SLOT_NUM; i++)
    {
        ASSERT_TRUE(__notnull__(objects[i]));
        delete objects[i];
    }

    object = new Obj(0);
    ASSERT_TRUE(__notnull__(object));
    delete object;
}
