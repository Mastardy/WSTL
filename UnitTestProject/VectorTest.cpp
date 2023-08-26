#include <gtest/gtest.h>
#include "../WSTL/WSTL.hpp"

using namespace WSTL;

TEST(VectorTest, DefaultConstructor)
{
    const Vector<char> a;
    EXPECT_EQ(a.Size(), 0);
    EXPECT_EQ(a.Capacity(), 0);
    EXPECT_THROW(a[0], std::out_of_range);

    const Vector<int> b(10);
    EXPECT_EQ(b.Size(), 10);
    EXPECT_EQ(b[0], 0);
}

TEST(VectorTest, FillConstructor)
{
    const Vector<int> a(10, 5);
    EXPECT_EQ(a.Size(), 10);
    EXPECT_EQ(a.Capacity(), 10);
    for(int i = 0; i < 10; i++)
    {
        EXPECT_EQ(a[i], 5);
    }
    EXPECT_THROW(a[10], std::out_of_range);
}

TEST(VectorTest, CopyConstructor)
{
    const Vector<int> a(10, 5);
    Vector<int> b(a);
    b[0] = 10;
    EXPECT_NE(b[0], a[0]);
    EXPECT_EQ(b[1], a[1]);
}

TEST(VectorTest, MoveConstructor)
{
    Vector<int> a(10, 5);
    Vector<int> b(std::move(a));

    EXPECT_TRUE(a.IsEmpty());
    EXPECT_EQ(b.Size(), 10);
    EXPECT_EQ(b.Capacity(), 10);
    for(int i = 0; i < 10; i++)
    {
        EXPECT_EQ(b[i], 5);
    }
}

TEST(VectorTest, InitListConstructor)
{
    Vector<int> a({1, 2, 3, 4, 5});
    EXPECT_EQ(a.Size(), 5);
    EXPECT_EQ(a.Capacity(), 5);
    for(int i = 0; i < 5; i++)
    {
        EXPECT_EQ(a[i], i + 1);
    }
    EXPECT_THROW(a[5], std::out_of_range);
}

TEST(VectorTest, CopyAssignment)
{
    const Vector<int> a(10, 5);
    Vector<int> b = a;
    b[0] = 10;
    EXPECT_NE(b[0], a[0]);
    EXPECT_EQ(b[1], a[1]);
}

TEST(VectorTest, MoveAssignment)
{
    Vector<int> a(10, 5);
    Vector<int> b = std::move(a);
    EXPECT_TRUE(a.IsEmpty());
    EXPECT_EQ(b.Size(), 10);
    EXPECT_EQ(b.Capacity(), 10);
    for(int i = 0; i < 10; i++)
    {
        EXPECT_EQ(b[i], 5);
    }
}

TEST(VectorTest, InitListAssignment)
{
    Vector<int> a = {1, 2, 3, 4, 5};
    EXPECT_EQ(a.Size(), 5);
    EXPECT_EQ(a.Capacity(), 5);
    for(int i = 0; i < 5; i++)
    {
        EXPECT_EQ(a[i], i + 1);
    }
    EXPECT_THROW(a[5], std::out_of_range);
}

TEST(VectorTest, AssignMethods)
{
    Vector<int> a;
    a.Assign(5, 10);
    EXPECT_EQ(a.Size(), 5);
    EXPECT_EQ(a[0], 10);
    
    Vector<int> b;
    b.Assign({1, 2, 3, 4, 5});
    EXPECT_EQ(b.Size(), 5);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[4], 5);

    Vector<int> c(5, 10);
    c.Assign(c.begin(), c.begin() + 2, 12);
    EXPECT_EQ(c.Size(), 5);
    EXPECT_EQ(c[0], 12);
    EXPECT_EQ(c[1], 12);
    EXPECT_EQ(c[2], 10);
}

struct TestStruct
{
    int a;
    int b;
    int c;
};

TEST(VectorTest, PushBack)
{
    Vector<int> a;
    a.PushBack(1);
    EXPECT_EQ(a.Size(), 1);
    EXPECT_EQ(a[0], 1);
    a.PushBack(2);
    EXPECT_EQ(a.Size(), 2);
    EXPECT_EQ(a[1], 2);

    int& b = a.PushBack();
    EXPECT_EQ(a.Size(), 3);
    b = 3;
    EXPECT_EQ(a[2], 3);

    const auto c = static_cast<int*>(a.PushBackUninitialized());
    EXPECT_EQ(a.Size(), 4);
    *c = 4;
    EXPECT_EQ(a[3], 4);

    int d = 5;
    a.PushBack(std::move(d));
    EXPECT_EQ(a.Size(), 5);
    EXPECT_EQ(a[4], 5);
}

TEST(VectorTest, PopBack)
{
    Vector<int> a({1, 2, 3, 4, 5});
    a.PopBack();
    EXPECT_EQ(a.Size(), 4);
    EXPECT_EQ(a[3], 4);
    EXPECT_THROW(a[4], std::out_of_range);
}

TEST(VectorTest, EmplaceBack)
{
    Vector<int> a;
    a.EmplaceBack(1);
    EXPECT_EQ(a.Size(), 1);
    EXPECT_EQ(a[0], 1);
    a.EmplaceBack(2);
    EXPECT_EQ(a.Size(), 2);
    EXPECT_EQ(a[1], 2);
}

TEST(VectorTest, Emplace)
{
    Vector<int> a({1, 2, 3, 4, 5});
    a.Emplace(1, 10);
    EXPECT_EQ(a.Size(), 6);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 10);
    EXPECT_EQ(a[2], 2);
}
TEST(VectorTest, Insert)
{
    Vector<int> a({1, 2, 3, 4, 5});
    a.Insert(1, 10);
    EXPECT_EQ(a.Size(), 6);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 10);
    EXPECT_EQ(a[2], 2);

    int b = 20;
    a.Insert(1, std::move(b));
    EXPECT_EQ(a.Size(), 7);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 20);
    EXPECT_EQ(a[2], 10);
    
    a.Insert(1, 2, 30);
    EXPECT_EQ(a.Size(), 9);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 30);
    EXPECT_EQ(a[2], 30);
    EXPECT_EQ(a[3], 20); 

    a.Insert(1, {40, 50, 60});
    EXPECT_EQ(a.Size(), 12);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 40);
    EXPECT_EQ(a[2], 50);
    EXPECT_EQ(a[3], 60);
    EXPECT_EQ(a[4], 30);
}

TEST(VectorTest, Erase)
{
    Vector<int> a = {1, 2, 3, 4, 5};
    a.Erase(1);
    EXPECT_EQ(a.Size(), 4);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 3);
    EXPECT_EQ(a[2], 4);

    a.Erase(a.begin() + 1, a.begin() + 3);
    EXPECT_EQ(a.Size(), 2); 
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 5); 
}

TEST(VectorTest, Clear)
{
    Vector<int> a = {1, 2, 3, 4, 5};
    a.Clear();
    EXPECT_EQ(a.Size(), 0);
    EXPECT_EQ(a.Capacity(), 0);
}

TEST(VectorTest, Swap)
{
    Vector<int> a = {1, 2, 3, 4, 5};
    Vector<int> b = {6, 7, 8, 9, 10};
    a.Swap(b);
    EXPECT_EQ(a.Size(), 5);
    EXPECT_EQ(b.Size(), 5);
    for(int i = 0; i < 5; i++)
    {
        EXPECT_EQ(a[i], i + 6);
        EXPECT_EQ(b[i], i + 1);
    }
}

TEST(VectorTest, Resize)
{
    Vector<int> a = {1, 2, 3, 4, 5};
    a.Resize(10);
    EXPECT_EQ(a.Size(), 10);
    for(int i = 0; i < 5; i++)
    {
        EXPECT_EQ(a[i], i + 1);
    }
    for(int i = 5; i < 10; i++)
    {
        EXPECT_EQ(a[i], 0);
    }

    Vector<int> b = {1, 2, 3, 4, 5};
    b.Resize(10, 10);
    EXPECT_EQ(b.Size(), 10);
    for(int i = 0; i < 5; i++)
    {
        EXPECT_EQ(b[i], i + 1);
    }
    for(int i = 5; i < 10; i++)
    {
        EXPECT_EQ(b[i], 10);
    }
}

TEST(VectorTest, SetCapacity)
{
    Vector<int> a = {1, 2, 3, 4, 5};
    a.SetCapacity(10);
    EXPECT_EQ(a.Size(), 5);
    EXPECT_EQ(a.Capacity(), 10);
    for(int i = 0; i < 5; i++)
    {
        EXPECT_EQ(a[i], i + 1);
    }
}

TEST(VectorTest, ShrinkToFit)
{
    Vector<int> a = {1, 2, 3, 4, 5};
    a.SetCapacity(10);
    a.ShrinkToFit();
    EXPECT_EQ(a.Capacity(), 5);
}