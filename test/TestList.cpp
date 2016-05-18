#include "gtest/gtest.h"
#include "ccinfra/container/list/List.h"
#include <ccinfra/container/list/ListElem.h>

namespace
{
    struct Elem : ListElem<Elem>
    {
        Elem(int a) : x(a)
        {
        }

        int getValue() const
        {
            return x;
        }

    private:
        int x;
    };
}

struct ListTest : public testing::Test
{
protected:
    List<Elem> elems;
};

TEST_F(ListTest, should_be_empty_when_init)
{
    ASSERT_TRUE(elems.isEmpty());
    ASSERT_EQ(0, elems.size());
}
