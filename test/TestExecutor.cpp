#include "gtest/gtest.h"
#include <ccinfra/sched/Executor.h>

namespace
{
    int fib(int n)
    {
        if(n == 0) return 0;
        return n + fib(n-1);
    }

    struct Fib
    {
        Fib(int n) : n(n)
        {
        }

        int operator()() const
        {
            return fib(n);
        }

    private:
        int n;
    };
}

struct ExecutorTest : testing::Test
{
protected:
    Executor executor{2};
};

TEST_F(ExecutorTest, should_execute_normal_function)
{
    auto result = executor.execute(fib, 5);
    ASSERT_EQ(15, result.get());
}

TEST_F(ExecutorTest, should_execute_lambda)
{
    const int num = 5;
    auto result = executor.execute([=](){ return fib(num); });
    ASSERT_EQ(15, result.get());
}

TEST_F(ExecutorTest, should_execute_functor)
{
    auto result = executor.execute(Fib(5));
    ASSERT_EQ(15, result.get());
}

TEST_F(ExecutorTest, should_execute_muti_task_concurrently)
{
    auto result1 = executor.execute(Fib(5));
    auto result2 = executor.execute(Fib(6));

    ASSERT_EQ(15, result1.get());
    ASSERT_EQ(21, result2.get());
}
