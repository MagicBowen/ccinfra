#include "gtest/gtest.h"
#include <ccinfra/mem/AutoMsg.h>

namespace
{
    struct LargeMsg
    {
        U8 msgBuff[16 * 1024];
    };
}

TEST(AutoMsgTest, should_alloc_auto_msg_not_in_stack)
{
    AutoMsg<LargeMsg> msg;

    ASSERT_TRUE(MsgAllocator::withIn(msg.getPointer()));
}
