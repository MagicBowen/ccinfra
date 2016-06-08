#include "gtest/gtest.h"
#include "ccinfra/ctnr/array/Array.h"

namespace
{
    struct PlainObject
    {
        int a;
        int b;
    };

    struct Object
    {
        enum { INVALID_VALUE = 0xFF };

        Object() : value(INVALID_VALUE)
        {
        }

        Object(int value)
        :value(value)
        {
        }

        int getValue() const
        {
            return value;
        }

    private:
        U8 value;
    };
}

TEST(ArrayTest, should_define_plain_object_array)
{
    Array<PlainObject, 2> objects;

    objects[0].a = 1;
    objects[1].b = 2;

    ASSERT_EQ(2, objects.size());
    ASSERT_EQ(1, objects[0].a);
    ASSERT_EQ(2, objects[1].b);
}
