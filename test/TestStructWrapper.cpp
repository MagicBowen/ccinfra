#include "gtest/gtest.h"
#include "ccinfra/base/BaseTypes.h"
#include "ccinfra/mem/StructWrapper.h"

namespace
{
    struct PlainMsg
    {
        U32 id;
        U32 transNum;
    };

    STRUCT_WRAPPER(DomainEvent, PlainMsg)
    {
        enum
        {
            MIN_ID = 0,
            MAX_ID = 20,
        };

        bool isValid() const
        {
            return (MIN_ID <= id) && (id <= MAX_ID);
        }
    };
}

TEST(StructWrapperTest, should_use_the_method_when_wrappered)
{
    PlainMsg msg{2, 4};

    DomainEvent& event = DomainEvent::by(msg);

    ASSERT_TRUE(event.isValid());
    ASSERT_EQ(4, event.transNum);
}
