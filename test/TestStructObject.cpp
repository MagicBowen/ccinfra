#include "magellan/magellan.hpp"
#include "ccinfra/mem/StructObject.h"
#include "ccinfra/base/BaseTypes.h"

USING_HAMCREST_NS

namespace
{
    struct Msg
    {
        U32 id;
        U32 transNum;
    };
}

FIXTURE(StructObjectTest)
{

	TEST("should_be_zero_when_init")
	{
		StructObject<Msg> msg;
		ASSERT_THAT(msg.id, eq(0));
		ASSERT_THAT(msg.transNum, eq(0));

		memset(&msg, 0xff, sizeof(msg));
		ASSERT_THAT(msg.id, eq(0xffffffff));
		ASSERT_THAT(msg.transNum, eq(0xffffffff));
	}

	TEST("should_be_used_as_pointer")
	{
		StructObject<Msg> msg;
		ASSERT_THAT(msg->id, eq(0));
		ASSERT_THAT(msg->transNum, eq(0));

		memset(msg.getPointer(), 0x0ff, sizeof(msg));
		ASSERT_THAT((*msg).id, eq(0xffffffff));
		ASSERT_THAT((*msg).transNum, eq(0xffffffff));
	}
};
