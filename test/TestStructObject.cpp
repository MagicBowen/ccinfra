#include "gtest/gtest.h"
#include "ccinfra/mem/StructObject.h"
#include "ccinfra/base/BaseTypes.h"

namespace
{
    struct Msg
    {
        U32 id;
        U32 transNum;
    };
}

TEST(StructObjectTest, should_be_zero_when_init)
{
    StructObject<Msg> msg;
    ASSERT_EQ(0, msg.id);
    ASSERT_EQ(0, msg.transNum);

    memset(&msg, 0xff, sizeof(msg));
    ASSERT_EQ(0xffffffff, msg.id);
    ASSERT_EQ(0xffffffff, msg.transNum);
}

TEST(StructObjectTest, should_be_used_as_pointer)
{
    StructObject<Msg> msg;
    ASSERT_EQ(0, msg->id);
    ASSERT_EQ(0, msg->transNum);

    memset(msg.getPointer(), 0x0ff, sizeof(msg));
    ASSERT_EQ(0xffffffff, (*msg).id);
    ASSERT_EQ(0xffffffff, (*msg).transNum);
}
