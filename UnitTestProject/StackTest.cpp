#include <gtest/gtest.h>
#include "../WSTL/WSTL.hpp"

using namespace WSTL;

TEST(StackTest, DefaultConstructor)
{
    const Stack<char> a;
    EXPECT_EQ(a.Size(), 0);
}

TEST(StackTest, Push)
{
    Stack<int> a;
    a.Push(5);
    a.Push(10);

    EXPECT_EQ(a.Size(), 2);
}

TEST(StackTest, Pop)
{
    Stack<int> a;
    a.Push(5);
    a.Push(10);

    EXPECT_EQ(a.Pop(), 10);
    EXPECT_EQ(a.Size(), 1);
}

TEST(StackTest, Top)
{
    Stack<int> a;
    a.Push(5);
    a.Push(10);

    EXPECT_EQ(a.Top(), 10);
    EXPECT_EQ(a.Size(), 2);
}

TEST(StackTest, IsEmpty)
{
    Stack<int> a;
    EXPECT_TRUE(a.IsEmpty());

    a.Push(5);
    EXPECT_FALSE(a.IsEmpty());
}

TEST(StackTest, Size)
{
    Stack<int> a;
    EXPECT_EQ(a.Size(), 0);

    a.Push(5);
    EXPECT_EQ(a.Size(), 1);
}