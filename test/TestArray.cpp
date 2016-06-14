#include "magellan/magellan.hpp"
#include "ccinfra/ctnr/array/Array.h"

USING_HAMCREST_NS

namespace
{
    struct PlainObject
    {
        int a;
        int b;
    };

    const int INVALID_VALUE = 0xFF;

    struct Object
    {
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

FIXTURE(ArrayTest)
{
	{
	    Array<PlainObject, 2> objects;

	    objects[0].a = 1;
	    objects[1].b = 2;

	    ASSERT_THAT(objects.size(), eq(2));
	    ASSERT_THAT(objects[0].a, eq(1));
	    ASSERT_THAT(objects[1].b, eq(2));
	}

	{
	    Array<Object, 2> objects;

	    ASSERT_THAT(objects[0].getValue(), eq(INVALID_VALUE));
	    ASSERT_THAT(objects[1].getValue(), eq(INVALID_VALUE));
	}

	{
	    Array<Object, 2> objects(5);

	    ASSERT_THAT(objects[0].getValue(), eq(5));
	    ASSERT_THAT(objects[1].getValue(), eq(5));
	}

	{
	    Array<Object, 2> objects(5);

	    objects[1].update(10);

	    ASSERT_THAT(objects[0].getValue(), eq(5));
	    ASSERT_THAT(objects[1].getValue(), eq(10));
	}

	{
	    Array<Object, 2> objects;

	    objects.emplace(1, 5);

	    ASSERT_THAT(objects[0].getValue(), eq(INVALID_VALUE));
	    ASSERT_THAT(objects[1].getValue(), eq(5));
	}

	{
	    typedef Array<Object, 2> ThisArray;
	    ThisArray objects;


	    ThisArray::Iterator i = objects.begin();
	    ASSERT_THAT(i->getValue(), eq(INVALID_VALUE));

	    objects[1].update(5);
	    i++;
	    ASSERT_THAT(i->getValue(), eq(5));

	    i->update(10);
	    ASSERT_THAT(objects[1].getValue(), eq(10));

	    ASSERT_THAT(i, ne(objects.end()));
	    ASSERT_THAT(++i, eq(objects.end()));
	}

	{
	    typedef Array<Object, 2> ThisArray;
	    ThisArray objects(10);

	    objects[1].update(5);

	    U32 sum = 0;
	    ARRAY_FOREACH(ThisArray, i, objects)
	    {
	        sum += i->getValue();
	    }
	    ASSERT_THAT(sum, eq(15));
	}
};


