#include <gtest/gtest.h>
#include "containers/Containers.hpp"

using namespace WSTL;

TEST(SListTest, DefaultConstructor)
{
    const SList<char> a;
    EXPECT_EQ(a.Size(), 0);
}

TEST(SListTest, FillConstructor)
{
    const SList<int> a(10, 5);
    EXPECT_EQ(a.Size(), 10);
}

TEST(SListTest, Copy)
{
    const SList<int> a(10, 5);
    const SList<int> b(a);

    EXPECT_EQ(b.Size(), a.Size());

    const SList<int> c = b;

    EXPECT_EQ(c.Size(), a.Size());
}

TEST(SListTest, AssignTest)
{
    SList<int> a(10, 0);
    a.Assign(0, 5);

    EXPECT_EQ(a.Size(), 11);
    EXPECT_EQ(a.Front(), 5);
}

TEST(SListTest, PushBack)
{
    SList<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushBack(i);
    }

    EXPECT_EQ(a.Size(), 10);
    EXPECT_EQ(a.Back(), 9);
    EXPECT_EQ(a.Front(), 0);
}

TEST(SListTest, PushFront)
{
    SList<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushFront(i);
    }

    EXPECT_EQ(a.Size(), 10);
    EXPECT_EQ(a.Back(), 0);
    EXPECT_EQ(a.Front(), 9);
}

TEST(SListTest, PopBack)
{
    SList<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushBack(i);
    }

    a.PopBack();

    EXPECT_EQ(a.Size(), 9);
    EXPECT_EQ(a.Front(), 0);
    EXPECT_EQ(a.Back(), 8);
}

TEST(SListTest, PopFront)
{
    SList<int> a;

    for(int i = 0; i < 10; i++)
    {
        a.PushBack(i);
    }

    a.PopFront();

    EXPECT_EQ(a.Size(), 9);
    EXPECT_EQ(a.Front(), 1);
    EXPECT_EQ(a.Back(), 9);
}

TEST(SListTest, Clear)
{
    SList<int> a(10, 5);
    a.Clear();

    EXPECT_EQ(a.Size(), 0);
}

TEST(SListTest, Remove)
{
    SList<int> a;

    for(int i = 0; i < 10; i++)
    { 
        a.PushBack(i);
    }
    
    a.Remove(8);
    a.Remove(8);
    
    EXPECT_EQ(a.Size(), 8);
    EXPECT_EQ(a.Back(), 7);
}

TEST(SListTest, BackAndFront)
{
    SList<int> a;

    EXPECT_THROW(a.Back(), std::out_of_range);
    EXPECT_THROW(a.Front(), std::out_of_range);

    for(int i = 0; i < 3; i++)
    {
        a.PushBack(i);
    }

    EXPECT_EQ(a.Back(), 2);
    EXPECT_EQ(a.Front(), 0);
}

TEST(SListTest, Emptiness)
{
    SList<int> a;

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