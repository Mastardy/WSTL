#include <gtest/gtest.h>
#include "WSTL/WSTL.hpp"

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

TEST(ListTest, AssignTest)
{
    List<int> a(10, 0);
    a.Assign(0, 5);

    EXPECT_EQ(a.Size(), 11);
    EXPECT_EQ(a.Front(), 5);
}

TEST(ListTest, PushBack)
{
    List<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushBack(i);
    }

    EXPECT_EQ(a.Size(), 10);
    EXPECT_EQ(a.Back(), 9);
    EXPECT_EQ(a.Front(), 0);
}

TEST(ListTest, PushFront)
{
    List<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushFront(i);
    }

    EXPECT_EQ(a.Size(), 10);
    EXPECT_EQ(a.Back(), 0);
    EXPECT_EQ(a.Front(), 9);
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
    EXPECT_EQ(a.Front(), 0);
    EXPECT_EQ(a.Back(), 8);
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
    EXPECT_EQ(a.Front(), 1);
    EXPECT_EQ(a.Back(), 9);
}

TEST(ListTest, Clear)
{
    List<int> a(10, 5);
    a.Clear();

    EXPECT_EQ(a.Size(), 0);
}

TEST(ListTest, Remove)
{
    List<int> a;

    for(int i = 0; i < 10; i++)
    { 
        a.PushBack(i);
    }
    
    a.Remove(8);
    a.Remove(8);
    
    EXPECT_EQ(a.Size(), 8);
    EXPECT_EQ(a.Back(), 7);
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