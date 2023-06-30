#include <gtest/gtest.h>
#include "../WSTL/WSTL.hpp"

using namespace WSTL;

TEST(ListTest, DefaultConstructor)
{
    const List<char> a;
    EXPECT_EQ(a.Size(), 0);
}

TEST(ListTest, FillConstructor)
{
    const List<int> a(10, 5);
    EXPECT_EQ(a.Size(), 10);
}

TEST(ListTest, Copy)
{
    const List<int> a(10, 5);
    const List<int> b(a);

    EXPECT_EQ(b.Size(), a.Size());

    const List<int> c = b;

    EXPECT_EQ(c.Size(), a.Size());
}

TEST(ListTest, BracketOperator)
{
    List<int> a;
    for(int i = 0; i < 10; i++)
    {
        a.PushBack(i);
    }

    EXPECT_EQ(a[3], 3);
    EXPECT_EQ(a.At(3), 3);
    EXPECT_THROW(a[11], std::out_of_range);
    EXPECT_THROW(a.At(11), std::out_of_range);

    const List<int> b(3, 2);

    EXPECT_EQ(b[2], 2);
    EXPECT_EQ(b.At(2), 2);
    EXPECT_THROW(b[3], std::out_of_range);
    EXPECT_THROW(b.At(3), std::out_of_range);
}

TEST(ListTest, AssignTest)
{
    List<int> a(10, 0);
    a.Assign(0, 5);

    EXPECT_EQ(a.Size(), 11);
    EXPECT_EQ(a[0], 5);
    EXPECT_EQ(a[1], 0);
}

TEST(ListTest, PushBack)
{
    List<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushBack(i);
    }

    EXPECT_EQ(a.Size(), 10);
    EXPECT_EQ(a[9], 9);
}

TEST(ListTest, PushFront)
{
    List<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushFront(i);
    }

    EXPECT_EQ(a.Size(), 10);
    EXPECT_EQ(a[8], 1);
}

TEST(ListTest, PopBack)
{
    List<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushBack(i);
    }

    a.PopBack();

    EXPECT_EQ(a.Size(), 9);
    EXPECT_EQ(a[8], 8);
}

TEST(ListTest, PopFront)
{
    List<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushBack(i);
    }

    a.PopFront();

    EXPECT_EQ(a.Size(), 9);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[8], 9);
}

TEST(ListTest, Clear)
{
    List<int> a(10, 5);
    a.Clear();

    EXPECT_EQ(a.Size(), 0);
    EXPECT_THROW(a[0], std::out_of_range);
}

TEST(ListTest, Remove)
{
    List<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushBack(i);
    }

    a.Remove(5);

    EXPECT_EQ(a.Size(), 9);
    EXPECT_EQ(a[4], 4);
    EXPECT_EQ(a[5], 6);
}

TEST(ListTest, BackAndFront)
{
    List<int> a;

    EXPECT_THROW(a.Back(), std::out_of_range);
    EXPECT_THROW(a.Front(), std::out_of_range);

    for(int i = 0; i < 3; i++)
    {
        a.PushBack(i);
    }

    EXPECT_EQ(a.Back(), 2);
    EXPECT_EQ(a.Front(), 0);
}

TEST(ListTest, Emptiness)
{
    List<int> a;

    EXPECT_TRUE(a.IsEmpty());

    a.PushBack(1);

    EXPECT_FALSE(a.IsEmpty());

    a.Remove(0);

    EXPECT_TRUE(a.IsEmpty());

    a.PushFront(1);

    EXPECT_FALSE(a.IsEmpty());

    a.Clear();

    EXPECT_TRUE(a.IsEmpty());
}