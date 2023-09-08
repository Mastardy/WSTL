#include <gtest/gtest.h>

#include "containers/Set.hpp"

typedef WSTL::Set<int> SetType;

TEST(SetTest, Constructor)
{
    const SetType set;
    EXPECT_EQ(set.Size(), 0);
}

TEST(SetTest, Copy)
{
    SetType set;
    set.Insert(1);
    set.Insert(2);
    set.Insert(3);

    const SetType set2(set);
    EXPECT_EQ(set2.Size(), set.Size());

    const SetType set3 = set;
    EXPECT_EQ(set3.Size(), set.Size());    
}

TEST(SetTest, Move)
{
    SetType set;
    set.Insert(1);
    set.Insert(2);
    set.Insert(3);

    SetType set2(std::move(set));
    EXPECT_EQ(set2.Size(), 3);
    EXPECT_EQ(set.Size(), 0);

    const SetType set3 = std::move(set2);
    EXPECT_EQ(set3.Size(), 3);
    EXPECT_EQ(set2.Size(), 0);
}

TEST(SetTest, Insert)
{
    SetType set;
    set.Insert(1);
    EXPECT_EQ(set.Size(), 1);
    set.Insert(2);
    EXPECT_EQ(set.Size(), 2);
    set.Insert(3);
    EXPECT_EQ(set.Size(), 3);

    set.Insert(1);
    EXPECT_EQ(set.Size(), 3);
}

TEST(SetTest, Access)
{
    SetType set;
    set.Insert(3);
    set.Insert(1);
    set.Insert(2);

    EXPECT_TRUE(set.Contains(1));
    EXPECT_TRUE(set.Contains(2));
    EXPECT_TRUE(set.Contains(3));
    EXPECT_FALSE(set.Contains(4));

    EXPECT_EQ(set[0], 1);
    EXPECT_EQ(set[1], 2);
    EXPECT_EQ(set[2], 3);
    EXPECT_THROW(set[3], std::out_of_range);
}

TEST(SetTest, Delete)
{
    SetType set;
    set.Insert(3);
    set.Insert(1);
    set.Insert(2);

    EXPECT_TRUE(set.Contains(1));
    EXPECT_TRUE(set.Contains(2));
    EXPECT_TRUE(set.Contains(3));
    EXPECT_FALSE(set.Contains(4));

    set.Delete(2);
    EXPECT_EQ(set.Size(), 2);
    EXPECT_TRUE(set.Contains(1));
    EXPECT_FALSE(set.Contains(2));
    EXPECT_TRUE(set.Contains(3));
    EXPECT_FALSE(set.Contains(4));

    set.Delete(1);
    EXPECT_EQ(set.Size(), 1);
    EXPECT_FALSE(set.Contains(1));
    EXPECT_FALSE(set.Contains(2));
    EXPECT_TRUE(set.Contains(3));
    EXPECT_FALSE(set.Contains(4));
    
    set.Delete(3);
    EXPECT_EQ(set.Size(), 0);
    EXPECT_FALSE(set.Contains(1));
    EXPECT_FALSE(set.Contains(2));
    EXPECT_FALSE(set.Contains(3));
    EXPECT_FALSE(set.Contains(4));
}

TEST(SetTest, Clear)
{
    SetType set;
    set.Insert(3);
    set.Insert(1);
    set.Insert(2);

    EXPECT_TRUE(set.Contains(1));
    EXPECT_TRUE(set.Contains(2));
    EXPECT_TRUE(set.Contains(3));
    EXPECT_FALSE(set.Contains(4));

    set.Clear();
    EXPECT_EQ(set.Size(), 0);
    EXPECT_FALSE(set.Contains(1));
    EXPECT_FALSE(set.Contains(2));
    EXPECT_FALSE(set.Contains(3));
    EXPECT_FALSE(set.Contains(4));
}

TEST(SetTest, ToVector)
{
    SetType set;
    set.Insert(3);
    set.Insert(1);
    set.Insert(2);

    const auto vec = set.ToVector();
    EXPECT_EQ(vec.Size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST(SetTest, MinMax)
{
    SetType set;
    set.Insert(3);
    set.Insert(1);
    set.Insert(2);

    EXPECT_EQ(set.Min(), 1);
    EXPECT_EQ(set.Max(), 3);
}

TEST(SetTest, Contains)
{
    SetType set;
    set.Insert(3);
    set.Insert(1);
    set.Insert(2);

    EXPECT_TRUE(set.Contains(1));
    EXPECT_TRUE(set.Contains(2));
    EXPECT_TRUE(set.Contains(3));
    EXPECT_FALSE(set.Contains(4));
}