#include <gtest/gtest.h>

#include "memory/SharedPointer.hpp"

TEST(SharedPointerTest, DefaultConstructor)
{
    WSTL::SharedPointer<int> ptr;
    EXPECT_EQ(ptr.Get(), nullptr);
}

TEST(SharedPointerTest, ConstructorWithNullptr)
{
    WSTL::SharedPointer<int> ptr(nullptr);
    EXPECT_EQ(ptr.Get(), nullptr);
}

TEST(SharedPointerTest, ConstructorWithPointer)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> ptr(pInt);
    EXPECT_EQ(ptr.Get(), pInt);
}

TEST(SharedPointerTest, CopyConstructor)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> ptr(pInt);
    WSTL::SharedPointer<int> ptr2(ptr);
    EXPECT_EQ(ptr.Get(), ptr2.Get());
}

TEST(SharedPointerTest, MoveConstructor)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> ptr(pInt);
    WSTL::SharedPointer<int> ptr2(std::move(ptr));
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(ptr2.Get(), pInt);
    EXPECT_EQ(ptr2.UseCount(), 1);
}

TEST(SharedPointerTest, CopyAssignment)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> ptr(pInt);
    WSTL::SharedPointer<int> ptr2 = ptr;
    EXPECT_EQ(ptr.Get(), ptr2.Get());
}

TEST(SharedPointerTest, MoveAssignment)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> ptr(pInt);
    WSTL::SharedPointer<int> ptr2 = std::move(ptr);
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(ptr2.Get(), pInt);
    EXPECT_EQ(ptr2.UseCount(), 1);
}

TEST(SharedPointerTest, Destructor)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> ptr(pInt);
    {
        WSTL::SharedPointer<int> ptr2(ptr);
        EXPECT_EQ(ptr.Get(), ptr2.Get());
        EXPECT_EQ(ptr.UseCount(), 2);
    }
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(ptr.UseCount(), 1);
}

TEST(SharedPointerTest, DereferenceOperator)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> const ptr(pInt);
    EXPECT_EQ(*ptr, 5);
}

TEST(SharedPointerTest, ArrowOperator)
{
    struct Test
    {
        int a;
        int b;
    };

    auto const pTest = new Test{ 5, 10 };
    WSTL::SharedPointer<Test> const ptr(pTest);

    EXPECT_EQ(ptr->a, 5);
    EXPECT_EQ(ptr->b, 10);
}

TEST(SharedPointerTest, Bool)
{
    WSTL::SharedPointer<int> sPtr;
    EXPECT_FALSE(sPtr);

    sPtr = new int(5);
    EXPECT_TRUE(sPtr);
}

TEST(SharedPointerTest, Get)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> const ptr(pInt);
    EXPECT_EQ(ptr.Get(), pInt);
}

TEST(SharedPointerTest, UseCount)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> const ptr(pInt);
    EXPECT_EQ(ptr.UseCount(), 1);

    {
        WSTL::SharedPointer<int> const ptr2(ptr);
        EXPECT_EQ(ptr.UseCount(), 2);
        EXPECT_EQ(ptr2.UseCount(), 2);
    }

    EXPECT_EQ(ptr.UseCount(), 1);
}

TEST(SharedPointerTest, Reset)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> ptr(pInt);
    EXPECT_EQ(ptr.UseCount(), 1);

    ptr.Reset();
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(ptr.UseCount(), 0);

    ptr = new int(10);
    EXPECT_EQ(ptr.UseCount(), 1);

    WSTL::SharedPointer<int> ptr2(new int(15));
    EXPECT_EQ(ptr2.UseCount(), 1);

    ptr2.Reset(ptr);
    EXPECT_EQ(ptr2.UseCount(), 2);
    EXPECT_EQ(ptr.UseCount(), 2);

    ptr2.Reset(new int(20));
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(ptr.UseCount(), 1);
}

TEST(SharedPointerTest, Swap)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> ptr(pInt);
    WSTL::SharedPointer<int> ptr2(new int(10));

    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(ptr2.UseCount(), 1);

    ptr.Swap(ptr2);
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(ptr2.Get(), pInt);
    EXPECT_EQ(*ptr2, 5);
}

TEST(SharedPointerTest, IsUnique)
{
    auto const pInt = new int(5);
    WSTL::SharedPointer<int> const ptr(pInt);
    EXPECT_TRUE(ptr.IsUnique());

    WSTL::SharedPointer<int> ptr2(ptr);
    EXPECT_FALSE(ptr.IsUnique());
    EXPECT_FALSE(ptr2.IsUnique());

    ptr2.Reset();
    EXPECT_TRUE(ptr.IsUnique());
    EXPECT_FALSE(ptr2.IsUnique());
}

TEST(SharedPointerTest, MakeShared)
{
    auto pInt = new int(5);
    WSTL::SharedPointer<int> ptr = WSTL::MakeShared<int>(5);
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(*ptr, 5);

    WSTL::SharedPointer<int> ptr2 = WSTL::MakeShared<int>(pInt);
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(ptr2.Get(), pInt);
}

TEST(SharedPointerTest, Arrays)
{
    auto const pInt = new int[5]{ 1, 2, 3, 4, 5 };
    WSTL::SharedPointer<int[]> ptr(pInt);
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
    EXPECT_EQ(ptr[3], 4);
    EXPECT_EQ(ptr[4], 5);

    WSTL::SharedPointer<int[]> ptr2(ptr);
    EXPECT_EQ(ptr.UseCount(), 2);
    EXPECT_EQ(ptr2.UseCount(), 2);
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
    EXPECT_EQ(ptr[3], 4);
    EXPECT_EQ(ptr[4], 5);
    EXPECT_EQ(ptr2[0], 1);
    EXPECT_EQ(ptr2[1], 2);
    EXPECT_EQ(ptr2[2], 3);
    EXPECT_EQ(ptr2[3], 4);
    EXPECT_EQ(ptr2[4], 5);

    ptr2.Reset();
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(ptr2.UseCount(), 0);
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
    EXPECT_EQ(ptr[3], 4);
    EXPECT_EQ(ptr[4], 5);

    ptr2 = ptr;
    EXPECT_EQ(ptr.UseCount(), 2);
    EXPECT_EQ(ptr2.UseCount(), 2);
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
    EXPECT_EQ(ptr[3], 4);
    EXPECT_EQ(ptr[4], 5);
    EXPECT_EQ(ptr2[0], 1);
    EXPECT_EQ(ptr2[1], 2);
    EXPECT_EQ(ptr2[2], 3);
    EXPECT_EQ(ptr2[3], 4);
    EXPECT_EQ(ptr2[4], 5);

    ptr2.Reset();
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(ptr2.UseCount(), 0);
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
    EXPECT_EQ(ptr[3], 4);
    EXPECT_EQ(ptr[4], 5);
}