#include "magellan/magellan.hpp"
#include <ccinfra/mem/AutoMsg.h>

USING_HAMCREST_NS

namespace
{
    struct LargeMsg
    {
        U8 msgBuff[16 * 1024];
    };
}

FIXTURE(AutoMsgTest)
{
	{
	    AutoMsg<LargeMsg> msg;

	    ASSERT_THAT(MsgAllocator::withIn(msg.getPointer()), be_true());
	}
};


