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

        void update(U8 v)
        {
            value = v;
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

TEST(ArrayTest, should_define_object_array_with_default_constructor)
{
    Array<Object, 2> objects;

    ASSERT_EQ(Object::INVALID_VALUE, objects[0].getValue());
    ASSERT_EQ(Object::INVALID_VALUE, objects[1].getValue());
}

TEST(ArrayTest, should_define_object_array_with_para_constructor)
{
    Array<Object, 2> objects(5);

    ASSERT_EQ(5, objects[0].getValue());
    ASSERT_EQ(5, objects[1].getValue());
}

TEST(ArrayTest, should_invoke_unconst_method_of_array_elem)
{
    Array<Object, 2> objects(5);

    objects[1].update(10);

    ASSERT_EQ(5, objects[0].getValue());
    ASSERT_EQ(10, objects[1].getValue());
}

TEST(ArrayTest, should_emplace_a_array_elem)
{
    Array<Object, 2> objects;

    objects.emplace(1, 5);

    ASSERT_EQ(Object::INVALID_VALUE, objects[0].getValue());
    ASSERT_EQ(5, objects[1].getValue());
}

TEST(ArrayTest, should_use_the_iterator_of_array)
{
    typedef Array<Object, 2> ThisArray;
    ThisArray objects;


    ThisArray::Iterator i = objects.begin();
    ASSERT_EQ(Object::INVALID_VALUE, i->getValue());

    objects[1].update(5);
    i++;
    ASSERT_EQ(5, i->getValue());

    i->update(10);
    ASSERT_EQ(10, objects[1].getValue());

    ASSERT_NE(objects.end(), i);
    ASSERT_EQ(objects.end(), ++i);
}

TEST(ArrayTest, should_travel_array)
{
    typedef Array<Object, 2> ThisArray;
    ThisArray objects(10);

    objects[1].update(5);

    U32 sum = 0;
    ARRAY_FOREACH(ThisArray, i, objects)
    {
        sum += i->getValue();
    }
    ASSERT_EQ(15, sum);
}
