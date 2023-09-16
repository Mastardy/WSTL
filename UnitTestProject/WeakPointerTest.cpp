#include <gtest/gtest.h>

#include "memory/Memory.hpp"

TEST(WeakPointerTest, Constructor)
{
    WSTL::SharedPointer<int> sp1(new int(5));
    WSTL::WeakPointer<int> wp1(sp1);

    EXPECT_EQ(wp1.WeakUseCount(), 1);
    EXPECT_EQ(sp1.UseCount(), wp1.UseCount());
}

TEST(WeakPointerTest, Copy)
{
    WSTL::SharedPointer<int> sp1(new int(5));
    WSTL::WeakPointer<int> wp1(sp1);
    WSTL::WeakPointer<int> wp2(wp1);
    WSTL::WeakPointer<int> wp3 = wp2;

    EXPECT_EQ(wp1.WeakUseCount(), 3);
    EXPECT_EQ(wp2.WeakUseCount(), 3);
    EXPECT_EQ(wp3.WeakUseCount(), 3);
    EXPECT_EQ(sp1.UseCount(), wp1.UseCount());
    EXPECT_EQ(sp1.UseCount(), wp2.UseCount());
    EXPECT_EQ(sp1.UseCount(), wp3.UseCount());
}

TEST(WeakPointerTest, Move)
{
    WSTL::SharedPointer<int> sp1(new int(5));
    WSTL::WeakPointer<int> wp1(sp1);
    WSTL::WeakPointer<int> wp2(std::move(wp1));
    WSTL::WeakPointer<int> wp3 = std::move(wp2);

    EXPECT_EQ(wp3.WeakUseCount(), 1);
    EXPECT_EQ(wp3.UseCount(), sp1.UseCount());
}

TEST(WeakPointerTest, Reset)
{
    WSTL::SharedPointer<int> sp1(new int(5));
    WSTL::WeakPointer<int> wp1(sp1);

    EXPECT_EQ(wp1.WeakUseCount(), 1);
    EXPECT_EQ(sp1.UseCount(), wp1.UseCount());

    wp1.Reset();

    EXPECT_EQ(wp1.WeakUseCount(), 0);
    EXPECT_EQ(sp1.UseCount(), 1);
    EXPECT_EQ(wp1.UseCount(), 0);
}

TEST(WeakPointerTest, Expired)
{
    WSTL::SharedPointer<int> sp1(new int(5));
    WSTL::WeakPointer<int> wp1(sp1);

    EXPECT_FALSE(wp1.Expired());

    sp1.Reset();

    EXPECT_TRUE(wp1.Expired());
}

TEST(WeakPointerTest, Lock)
{
    WSTL::SharedPointer<int> sp1(new int(5));
    WSTL::WeakPointer<int> wp1(sp1);

    EXPECT_EQ(wp1.WeakUseCount(), 1);
    EXPECT_EQ(sp1.UseCount(), wp1.UseCount());

    auto temp = wp1.Lock();

    EXPECT_EQ(wp1.WeakUseCount(), 1);
    EXPECT_EQ(sp1.UseCount(), wp1.UseCount());
    EXPECT_EQ(*temp, 5);
}

TEST(WeakPointerTest, Swap)
{
    WSTL::SharedPointer<int> sp1(new int(5));
    WSTL::SharedPointer<int> sp2(new int(10));
    WSTL::WeakPointer<int> wp1(sp1);
    WSTL::WeakPointer<int> wp2(sp2);
    
    wp1.Swap(wp2);

    EXPECT_EQ(*wp1.Lock(), 10);
    EXPECT_EQ(*wp2.Lock(), 5);
}